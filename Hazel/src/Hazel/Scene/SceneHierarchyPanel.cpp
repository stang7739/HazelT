//
// Created by stang on 25-8-12.
//

#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

namespace Hazel
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender(Timestep ts)
    {
    	m_Times = m_Times <=360 ? m_Times + 1  : 0;
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity{entityID, m_Context.get()};
            DrawEntityNode(entity);
        });
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

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
    }

	static void DrawVec3Control(const std::string& label,glm::vec3& values,float resetValue = 0.0f,float columnWidth = 100.0f)
    {
	    ImGui::PushID(label.c_str());

    	ImGui::Columns(2);
    	ImGui::SetColumnWidth(0, columnWidth);
    	ImGui::Text(label.c_str());
    	ImGui::NextColumn();

    	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2{0,0});

    	float lineHeight = ImGui::GetFrameHeight();
    	// float lineHeight = 3;
    	ImVec2 buttomSize = ImVec2{lineHeight + 3.f, lineHeight};

    	ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.8f,0.1f,0.15f,1.0f});
    	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.9,0.2f,0.2f,1.0f});
    	ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.8f,0.1f,0.15f,1.0f});
    	if(ImGui::Button("X",buttomSize))
		{
			values.x = resetValue;
		}
    	ImGui::PopStyleColor(3);

    	ImGui::SameLine();
    	ImGui::DragFloat("##X", &values.x, 0.1f, 0.f, 0.f, "%.3f");
    	ImGui::PopItemWidth();
    	ImGui::SameLine();

    	ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.2f,0.7f,0.2f,1.0f});
    	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.3,0.8f,0.3f,1.0f});
    	ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.2f,0.7f,0.2f,1.0f});
    	if(ImGui::Button("Y",buttomSize))
    		values.y = resetValue;
    	ImGui::PopStyleColor(3);

    	ImGui::SameLine();
    	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.f, 0.f, "%.3f");
		ImGui::PopItemWidth();
    	ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4{0.1f,0.25f,0.8f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4{0.2f,0.35f,0.9f,1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4{0.1f,0.25f,0.8f,1.0f});
		if(ImGui::Button("Z",buttomSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.f, 0.f, "%.3f");
    	ImGui::PopItemWidth();
    	ImGui::PopStyleVar();

    	ImGui::Columns(1);

    	ImGui::PopID();
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				auto tag = entity.GetComponent<TagComponent>();
				DrawVec3Control("Translation", tc.Translation, 0.0f);
				// glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", tc.Rotation, 0.0f);
				if(tag.Tag != "Camera A" && tag.Tag != "Camera B")
				 tc.Rotation.y = m_Times;
				DrawVec3Control("Scale", tc.Scale, 1.0f, 100.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
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

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}


				ImGui::TreePop();
			}
		}
    	if(entity.HasComponent<SpriteRendererComponent>())
    	{
    		if(ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Sprite Renderer"))
    		{
    			auto& src = entity.GetComponent<SpriteRendererComponent>();
    			ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
    			ImGui::TreePop();
    		}
    	}
	}
}
