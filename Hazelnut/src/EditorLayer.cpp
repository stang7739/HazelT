//
// Created by stang on 25-7-11.
//

#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Renderer/Renderer2D.h"
#include <chrono>
#include <ImGuizmo.h>


#include "Hazel/Renderer/Framebuffer.h"
#include "Hazel/Scene/Component.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/SceneCameraController.h"
#include "Hazel/Scene/SceneSerializer.h"
#include "Hazel/Utils/PlatformUtils.h"

namespace Hazel
{
    // Helper function to safely convert texture ID to void pointer for ImGui
    inline void* TextureIDToImGuiHandle(uint32_t textureID)
    {
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

    EditorLayer::EditorLayer(): Layer("EditorLayer"), m_CameraController(1260.f / 720.f, true),
                                m_SquareColor(1, 1, 1, 1.f)
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
#if 0
        auto square = m_ActiveScene->CreateEntity("Green Square", glm::vec3{0.5f, 0.0f, -1.0f});
        square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

        auto redSquare = m_ActiveScene->CreateEntity("Red Square", glm::vec3{-0.5f, 0.0f, -2.0f});
        redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

        m_SquareEntity = square;
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera A", glm::vec3{0.0f, 0.0f, 0.0f});
        m_CameraEntity.AddComponent<CameraComponent>();

        m_SecondCamera = m_ActiveScene->CreateEntity("Camera B", glm::vec3{0.0f, 0.0f, 0.1f});
        auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
        cc.Primary = false;
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    } //Executed when the layer is loaded into the stack
    void EditorLayer::OnDetach()
    {
    } //Executed when the layer is removed from the stack
    void EditorLayer::OnUpdate(Timestep timestep)
    {
        m_timestep =timestep;
        m_Rotation = m_Rotation <= 180 ? (m_Rotation += 1 * m_Speed) : 0;
        HZ_PROFILE_FUNCTION();
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height !=
                m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,(uint32_t)m_ViewportSize.y);
        }
        if (m_ViewportFocused)
            m_CameraController.OnUpdate(timestep);
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        {
            HZ_PROFILE_SCOPE("CameraController::OnUpdate");
            // PROFILE_SCOPE("CameraController::OnUpdate");
            // m_CameraController.OnUpdate(timestep);
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
            RenderCommand::Clear();
        }
        {
            // HZ_INFO("{}",m_Rotation);
            HZ_PROFILE_SCOPE("Renderer Draw");
            m_ActiveScene->OnUpdate(timestep);
            // Renderer2D::EndScene();
            m_Framebuffer->Unbind();
        }
    } //Update logic every frame

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
            ImGuiStyle& style = ImGui::GetStyle();
            float minWindowSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            style.WindowMinSize.x = minWindowSizeX;
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if(ImGui::MenuItem("New","Ctrl+N"))
                    {
                        NewSence();
                        // SceneSerializer serializer(m_ActiveScene);
                        // serializer.Serialize("assets/scenes/Example.hazel");
                    }

                    if(ImGui::MenuItem("Open...","Ctrl+O"))
                    {
                        OpenSence();
                        // SceneSerializer serializer(m_ActiveScene);
                        // serializer.Deserialize("assets/scenes/Example.hazel");
                    }
                    if(ImGui::MenuItem("Save As...","Ctrl+Shift+S"))
                    {
                        SaveSenceAs();
                    }
                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            m_SceneHierarchyPanel.OnImGuiRender(m_timestep);

            ImGui::Begin("Stats");

            auto stats = Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            // if (m_SquareEntity)
            // {
            //     ImGui::Separator();
            //     auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
            //     ImGui::Text("%s", tag.c_str());
            //     auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
            //     ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
            //     ImGui::Separator();
            // }
            //
            // ImGui::DragFloat3("Camera Transform",
            //             glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
            //
            // if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
            // {
            //     m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
            //     m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
            // }
            // {
            //     auto& camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
            //     float orthoSize = camera.GetOrthographicSize();
            //
            //     if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
            //         camera.SetOrthographicSize(orthoSize);
            // }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if(selectedEntity && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                float windowWidth = (float)ImGui::GetWindowWidth();
                float windowHeight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,windowWidth,windowHeight);
                // Camera
                auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
                const auto& camera = cameraEntity.GetComponent<CameraComponent>();
                const glm::mat4& cameraProjection = camera.Camera.GetProjection();
            }
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
            {
                // m_Framebuffer->Resize((uint32_t)viewportPanelSize.x,(uint32_t)viewportPanelSize.y);
                m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
                // HZ_INFO("{},{}",viewportPanelSize.x,viewportPanelSize.y);
                // m_SecondCamera.GetComponent<CameraComponent>().Camera.SetViewportsize(viewportPanelSize.x,viewportPanelSize.y);
                // m_CameraController.OnResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
            }
            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
            ImGui::Image(textureID, ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

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
    void EditorLayer::OnEvent(Event& event)
    {
        HZ_PROFILE_FUNCTION();
        m_CameraController.OnEvent(event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
    } //Respond to events that are distributed by the event system

    bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        if(e.GetRepeatCount() > 0)
            return false;
        bool control = Input::IsKeyPressed(HazelKey::LeftControl) || Input::IsKeyPressed(HazelKey::RightControl);
        bool shift = Input::IsKeyPressed(HazelKey::LeftShift) || Input::IsKeyPressed(HazelKey::RightShift);
        switch(e.GetKeyCode())
        {
        case HazelKey::N:
            {
                if(control)
                    NewSence();
                break;
            }
        case HazelKey::O:
            {
                if(control)
                    OpenSence();
                break;
            }
        case HazelKey::S:
            {
                if(control && shift)
                    SaveSenceAs();
            }

        }
        return false;
    }
    void EditorLayer::NewSence()
    {
        HZ_INFO("New Sence");
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,(uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OpenSence()
    {
        HZ_INFO("OpenSence");
        std::optional<std::string> filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
        if(filepath)
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,(uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(*filepath);
        }

    }
    void EditorLayer::SaveSenceAs()
    {
        HZ_INFO("SaveSenceAs");
        std::optional<std::string> filepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
        if(filepath)
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(*filepath);
        }

    }
}
