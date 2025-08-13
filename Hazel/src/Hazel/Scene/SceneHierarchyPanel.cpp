//
// Created by stang on 25-8-12.
//

#include "SceneHierarchyPanel.h"

#include <imgui.h>
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
    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
        {
            Entity entity{entityID, m_Context.get()};
            DrawEntityNode(entity);
        });
        if(ImGui::IsMouseDown(0)&&ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();

        ImGui::Begin("Properties");
        if(m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }
        ImGui::End();
    }
    void SceneHierarchyPanel::DrawEntityNode(Entity entityID)
    {
        // 假设：entityID 是你的实体句柄（可转换为 uint32_t）
        // 假设：TagComponent 含有 std::string Tag 作为实体名称/标签

        // 1) 取出实体的可读名称（在层级面板中显示）
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
        if (ImGui::IsItemClicked()) {
            m_SelectionContext = entityID;
        }

        // 5) 若外层节点被展开，则绘制其子节点
        if (outerOpened) {
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
            if (childOpened) {
                ImGui::TreePop(); // 与 childOpened 的 TreeNodeEx 成对
            }

            ImGui::TreePop(); // 与 outerOpened 的 TreeNodeEx 成对
        }
    }
    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if(entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;
            char buffer[256];
            memset(buffer,0,sizeof(buffer));
            strcpy_s(buffer,sizeof(buffer),tag.c_str());
            if(ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),ImGuiTreeNodeFlags_DefaultOpen,"Transform"))
            {
                auto& transform = entity.GetComponent<TransformComponent>().Transform;
                ImGui::DragFloat3("Position",glm::value_ptr(transform[3]),0.1f);
                ImGui::TreePop(); // 与 outerOpened 的 TreeNodeEx 成对

            }
        }
    }



}
