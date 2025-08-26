//
// Created by stang on 25-8-12.
//

#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"
#include <cstring>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
  #define _CRT_SECURE_NO_WARNINGS
#endif
namespace Hazel
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender(Timestep ts)
    {
        m_Times = m_Times <= 360 ? m_Times + 1 : 0;
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity{entityID, m_Context.get()};
            DrawEntityNode(entity);
        });
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                m_Context->CreateEntity("Empty Entity");
            }
            ImGui::EndPopup();
        }
        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entityID)
    {
        auto& tag = entityID.GetComponent<TagComponent>().Tag;

        // 2) 组装树节点标志：
        //    - 若该实体是当前选中实体，则添加“选中”高亮
        //    - OpenOnArrow：只有点击三角箭头才会切换展开/折叠（点击文本不切换）
        ImGuiTreeNodeFlags flags =
            ((m_SelectionContext == entityID) ? ImGuiTreeNodeFlags_Selected : 0) |
            ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        // 3) 绘制第一级树节点（返回是否“处于展开状态”）
        //    参数1：唯一 ID。这里把 entityID(32位) -> uint64_t -> void* 作为 ImGui 的内部 ID，
        //           避免仅用文本标签导致的 ID 冲突（多个节点同名时很重要）。
        //    参数2：标志位
        //    参数3：显示的文本（实体名）
        bool outerOpened = ImGui::TreeNodeEx(
            (void*)(uint64_t)(uint32_t)entityID,
            flags,
            tag.c_str()
        );

        // 4) 如果刚刚那个项被点击，则更新“当前选中实体”
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entityID;
        }
        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;
            ImGui::EndPopup();
        }


        // 5) 若外层节点被展开，则绘制其子节点
        if (outerOpened)
        {
            // 子节点标志：同样采用“点击箭头才开合”
            ImGuiTreeNodeFlags childFlags = ImGuiTreeNodeFlags_OpenOnArrow;

            // 6) 绘制一个子节点示例
            //    这里用一个固定常量 9817239 转成 void* 当作唯一 ID（仅示例，不够健壮）。
            //    实际项目里应为每个子项生成稳定的唯一 ID（或使用 PushID/PopID）。
            bool childOpened = ImGui::TreeNodeEx(
                (void*)9817239,
                childFlags,
                tag.c_str() // 示意：这里复用同一文本
            );

            // 7) 如果子节点也展开，在这里绘制子节点的内容……
            if (childOpened)
            {
                ImGui::TreePop(); // 与 childOpened 的 TreeNodeEx 成对
            }

            ImGui::TreePop(); // 与 outerOpened 的 TreeNodeEx 成对
        }
        if (entityDeleted)
        {
            m_Context->DestoryEntity(entityID);
            if (m_SelectionContext == entityID)
                m_SelectionContext = {};
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f,
                                float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float lineHeight = ImGui::GetFrameHeight();
        // float lineHeight = 3;
        ImVec2 buttomSize = ImVec2{lineHeight + 3.f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttomSize))
        {
            values.x = resetValue;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.f, 0.f, "%.3f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttomSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.f, 0.f, "%.3f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttomSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.f, 0.f, "%.3f");
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template <typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uifunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentReginAvailable = ImGui::GetContentRegionAvail();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;

            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags,
                                          name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentReginAvailable.x - lineHeight *0.5f);
            if (ImGui::Button("+", ImVec2{lineHeight, lineHeight}))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                    removeComponent = true;
                ImGui::EndPopup();
            }
            if (open)
            {
                // auto& src = entity.GetComponent<SpriteRendererComponent>();
                // ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
                uifunction(component);
                ImGui::TreePop();
            }
            if (removeComponent)
                entity.RemoveComponent<T>();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }
        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");
        }
        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                if (!m_SelectionContext.HasComponent<CameraComponent>())
                    m_SelectionContext.AddComponent<CameraComponent>();
                else
                    HZ_CORE_WARN("This entity already has the Camera Component!");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    m_SelectionContext.AddComponent<SpriteRendererComponent>();
                else
                    HZ_CORE_WARN("This entity already has the Sprite Renderer Component!");

                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
        {
            DrawVec3Control("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        });


        DrawComponent<CameraComponent>("Camera", entity, [&](auto& component)
        {
            auto& camera = component.Camera;

            ImGui::Checkbox("Primary", &component.Primary);

            const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                    {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
                if (ImGui::DragFloat("Vertical FOV", &verticalFov))
                    camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

                float orthoNear = camera.GetPerspectiveNearClip();
                if (ImGui::DragFloat("Near", &orthoNear))
                    camera.SetPerspectiveNearClip(orthoNear);

                float orthoFar = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Far", &orthoFar))
                    camera.SetPerspectiveFarClip(orthoFar);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                float orthoSize = camera.GetOrthographicSize();
                if (ImGui::DragFloat("Size", &orthoSize))
                    camera.SetOrthographicSize(orthoSize);

                float orthoNear = camera.GetOrthographicNearClip();
                if (ImGui::DragFloat("Near", &orthoNear))
                    camera.SetOrthographicNearClip(orthoNear);

                float orthoFar = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Far", &orthoFar))
                    camera.SetOrthographicFarClip(orthoFar);

                ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
            }
        });


        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](auto& component)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        });
    }
}
