//
// Created by stang on 25-7-11.
//

#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Renderer/Renderer2D.h"
#include <chrono>

#include "Hazel/Renderer/Framebuffer.h"
#include "Hazel/Scene/Component.h"
#include "Hazel/Scene/Scene.h"

namespace Hazel
{
    // Helper function to safely convert texture ID to void pointer for ImGui
    inline void* TextureIDToImGuiHandle(uint32_t textureID) {
        return reinterpret_cast<void*>(static_cast<intptr_t>(textureID));
    }


    template <typename Fn>
    class Timer
    {
    public:
        Timer(const char* name, Fn&& func): m_Name(name), m_Func(func), m_Stoppped(false)
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }

        ~Timer()
        {
            if (!m_Stoppped)
            {
                Stop();
            }
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().
                count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().
                count();

            m_Stoppped = true;

            float duration = (end - start) * 0.001f; // Convert to milliseconds
            m_Func({m_Name, duration});
        }

    private:
        const char* m_Name;
        Fn m_Func;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
        bool m_Stoppped;
    };

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name,[&](ProfileResult profileResult ){m_ProfileResults.push_back(profileResult);})

    EditorLayer::EditorLayer(): Layer("EditorLayer"), m_CameraController(1260.f / 720.f, true), m_SquareColor(1, 1, 1, 1.f)
    // Initialize the camera with orthographic projection
    {
        HZ_PROFILE_FUNCTION();
        Renderer2D::Init();
    }

    void EditorLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();
        m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");
        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        m_TextureStairs = SubTexture2D::CreateFromCoords(m_CheckerboardTexture, {0, 0}, {1, 1}, {0.2f, 0.2f});
        FramebufferSpecification fbspec;
        fbspec.Width = 1280;
        fbspec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbspec);
        m_ActiveScene = CreateRef<Scene>();
        
        // // 调试：检查默认构造的实体状态
        // HZ_CORE_INFO("=== EditorLayer Debug ===");
        // HZ_CORE_INFO("Before CreateEntity - m_SquareEntity valid: {}", (bool)m_SquareEntity);
        //
        // // 创建实体并立即检查
        // HZ_CORE_INFO("Calling CreateEntity...");
        
        // 测试：先用临时变量接收，然后再赋值
        Entity tempEntity = m_ActiveScene->CreateEntity("Green Square");
        // HZ_CORE_INFO("CreateEntity returned to temp variable");
        
        // 检查临时变量
        // HZ_CORE_INFO("Temp entity - Valid: {}, EntityID: {}",
        //              (bool)tempEntity, (uint32_t)tempEntity.GetEntityHandle());
        // HZ_CORE_INFO("Temp entity components - Transform: {}, Tag: {}, Sprite: {}",
        //              tempEntity.HasComponent<TransformComponent>(),
        //              tempEntity.HasComponent<TagComponent>(),
        //              tempEntity.HasComponent<SpriteRendererComponent>());
        //
        // 现在赋值给成员变量

        // HZ_CORE_INFO("Assigned temp to member variable");
        //
        // // 详细检查返回的实体
        // HZ_CORE_INFO("=== EditorLayer Entity Debug ===");
        // HZ_CORE_INFO("Scene address: {}", (void*)m_ActiveScene.get());
        // HZ_CORE_INFO("Returned entity - Valid: {}, EntityID: {}",
        //              (bool)m_SquareEntity,
        //              (uint32_t)m_SquareEntity.GetEntityHandle());
        //
        // // 检查实体内部的 Scene 指针
        // HZ_CORE_INFO("Entity's scene pointer: {}", (void*)m_SquareEntity.GetScene());
        //
        // 重新检查各种组件
        // bool hasTransform = m_SquareEntity.HasComponent<TransformComponent>();
        // bool hasTag = m_SquareEntity.HasComponent<TagComponent>();
        // bool hasSprite = m_SquareEntity.HasComponent<SpriteRendererComponent>();
        
        // HZ_CORE_INFO("Component check - Transform: {}, Tag: {}, Sprite: {}",
        //              hasTransform, hasTag, hasSprite);
        //
        // // 如果有 Sprite 组件，尝试获取它看看是怎么来的
        // if (hasSprite) {
        //     HZ_CORE_WARN("UNEXPECTED: Entity has SpriteRendererComponent!");
        //     auto& sprite = m_SquareEntity.GetComponent<SpriteRendererComponent>();
        //     HZ_CORE_INFO("Sprite color: ({}, {}, {}, {})",
        //                  sprite.Color.r, sprite.Color.g, sprite.Color.b, sprite.Color.a);
        // }
        //
        // HZ_CORE_INFO("=== End EditorLayer Debug ===");
        //
        // // 安全方式：如果没有组件就添加，如果有就获取并修改
        // if (!m_SquareEntity.HasComponent<SpriteRendererComponent>()) {
        //     HZ_CORE_INFO("Adding SpriteRendererComponent...");
        //     m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
        //     HZ_CORE_INFO("SpriteRendererComponent added successfully");
        // } else {
        //     HZ_CORE_WARN("Entity already has SpriteRendererComponent, updating color...");
        //     auto& sprite = m_SquareEntity.GetComponent<SpriteRendererComponent>();
        //     sprite.Color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
        //     HZ_CORE_WARN("Color updated");
        // }

        tempEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
        m_SquareEntity = tempEntity;

    } //Executed when the layer is loaded into the stack
    void EditorLayer::OnDetach()
    {
    } //Executed when the layer is removed from the stack
    void EditorLayer::OnUpdate(Timestep timestep)
    {
        m_Rotation = m_Rotation <= 180 ? (m_Rotation += 1 * m_Speed) : 0;
        HZ_PROFILE_FUNCTION();
        if(FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        }
        // PROFILE_SCOPE("EditorLayer::OnUpdate");
        if(m_ViewportFocused)
            m_CameraController.OnUpdate(timestep);
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        {
            HZ_PROFILE_SCOPE("CameraController::OnUpdate");
            // PROFILE_SCOPE("CameraController::OnUpdate");
            m_CameraController.OnUpdate(timestep);
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
            RenderCommand::Clear();
        }

        {
            HZ_PROFILE_SCOPE("Renderer Prep");
            // Renderer::BeginScene(m_CameraController.GetCamera());
            Renderer2D::BeginScene(m_CameraController.GetCamera());
        }

        {
            // HZ_INFO("{}",m_Rotation);
            HZ_PROFILE_SCOPE("Renderer Draw");
           m_ActiveScene->OnUpdate(timestep);
            Renderer2D::EndScene();
            m_Framebuffer->Unbind();
        }
    } //Update logic every frame
    void EditorLayer::OnEvent(Event& event)
    {
        HZ_PROFILE_FUNCTION();
        m_CameraController.OnEvent(event);
    } //Respond to events that are distributed by the event system
    void EditorLayer::OnImGuiRender()
    {
        HZ_PROFILE_FUNCTION();
        // Note: Switch this to true to enable dockspace
        static bool dockingEnabled = true;;
        if (dockingEnabled)
        {
            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::Begin("Settings");

            auto stats = Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            if(m_SquareEntity)
            {
                ImGui::Separator();
                auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
                ImGui::Text("%s",tag.c_str());
                auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
                ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
                ImGui::Separator();
            }
            // ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
            //
            // uint32_t textureColorID = m_Framebuffer->GetColorAttachmentRendererID();
            // ImGui::Image(TextureIDToImGuiHandle(textureColorID), ImVec2{1280.0f, 720.0f});
            //
            // textureID = m_Framebuffer->GetDepthAttachmentRendererID();
            //
            // ImGui::Image(TextureIDToImGuiHandle(textureID), ImVec2{1280.0f, 720.0f});
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            if(m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
            {
                // m_Framebuffer->Resize((uint32_t)viewportPanelSize.x,(uint32_t)viewportPanelSize.y);
                m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
                // m_CameraController.OnResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
            }
            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)textureID,ImVec2{m_ViewportSize.x,m_ViewportSize.y},ImVec2{0,1},ImVec2{1,0});
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::End();
        }
        else
        {
            ImGui::Begin("EditorLayer");
            ImGui::ColorEdit4("squre Color", glm::value_ptr(m_SquareColor));
            ImGui::SliderFloat("Rotation speed", &m_Speed, 0.0f, 10.0f);
            ImGui::InputInt("background squares", &m_speedsquare);
            ImGui::InputInt("Number of squares", &m_Count);

            for (auto& result : m_ProfileResults)
            {
                char label[50];
                strcpy(label, "%.3fms ");
                strcat(label, result.Name);
                ImGui::Text(label, result.Time);
            }
            auto stats = Renderer2D::GetStats();
            ImGui::Text("DrawCall: %d", stats.DrawCalls);
            ImGui::Text("QuadCount: %d", stats.QuadCount);
            ImGui::Text("VertexCount: %d", stats.GetTotalVertexCount());
            ImGui::Text("IndexCount: %d", stats.GetTotalIndexCount());
            m_ProfileResults.clear();
            ImGui::End();
        }
    } //Draw the ImGui interface
}
