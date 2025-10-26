//
// Created by stang on 25-7-11.
//

#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Renderer/Renderer2D.h"
#include <chrono>
#include <ImGuizmo.h>
#include "Hazel/Math/Math.h"
#include "Hazel/Renderer/Framebuffer.h"
#include "Hazel/Scene/Component.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/SceneSerializer.h"
#include "Hazel/Utils/PlatformUtils.h"

namespace Hazel
{
    // Helper function to safely convert texture ID to void pointer for ImGui
    inline void* TextureIDToImGuiHandle(uint32_t textureID)
    {
        return reinterpret_cast<void*>(static_cast<intptr_t>(textureID));
    }

    extern const std::filesystem::path g_AssetPath;

    EditorLayer::EditorLayer(): Layer("EditorLayer"), m_CameraController(1260.f / 720.f, true),
                                m_SquareColor(1, 1, 1, 1.f)
    {
        // Initialize the camera with orthographic projection
        HZ_PROFILE_FUNCTION();
        Renderer2D::Init();
    }

    void EditorLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();
        m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");
        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        m_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
        m_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");
        m_TextureStairs = SubTexture2D::CreateFromCoords(m_CheckerboardTexture, {0, 0}, {1, 1}, {0.2f, 0.2f});
        FramebufferSpecification fbspec;
        fbspec.Width = 1280;
        fbspec.Height = 720;
        fbspec.Attachments = {
            FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth
        };
        m_Framebuffer = Framebuffer::Create(fbspec);
        m_ActiveScene = CreateRef<Scene>();
        m_EditorCamera = EditorCamera(30.f, 1.778f, 0.1f, 1000.f);
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
        // m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    } //Executed when the layer is loaded into the stack
    void EditorLayer::OnDetach()
    {
    } //Executed when the layer is removed from the stack
    void EditorLayer::OnUpdate(Timestep timestep)
    {
        m_timestep = timestep;
        m_Rotation = m_Rotation <= 180 ? (m_Rotation += 1 * m_Speed) : 0;
        HZ_PROFILE_FUNCTION();
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height !=
                m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }


        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        {
            HZ_PROFILE_SCOPE("CameraController::OnUpdate");
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
            RenderCommand::Clear();
            m_Framebuffer->ClearAttachment(1, -1);
        }
        {
            HZ_PROFILE_SCOPE("Renderer Draw");
            // m_ActiveScene->OnUpdate(timestep);
            switch (m_SceneState)
            {
            case SceneState::Edit:
                {
                    m_EditorCamera.OnUpdate(timestep);

                    if (m_ViewportFocused)
                        m_CameraController.OnUpdate(timestep);
                    m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);
                    break;
                }
            case SceneState::Play:
                {
                    m_ActiveScene->OnUpdateRuntime(timestep);
                    break;
                }
            }
            auto [mx,my] = ImGui::GetMousePos();
            mx -= m_ViewportBounds[0].x;
            my -= m_ViewportBounds[0].y;
            glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
            my = viewportSize.y - my;
            int mouseX = (int)mx;
            int mouseY = (int)my;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
                m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
            }
            // HZ_INFO("{},{}", mouseX, mouseY);
            m_Framebuffer->Unbind();
        }
    } //Update logic every frame

    void EditorLayer::OnImGuiRender()
    {
        HZ_PROFILE_FUNCTION();
        // Note: Switch this to true to enable dockspace
        static bool dockingEnabled = true;
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

                    if (ImGui::MenuItem("New", "Ctrl+N"))
                    {
                        NewSence();
                        // SceneSerializer serializer(m_ActiveScene);
                        // serializer.Serialize("assets/scenes/Example.hazel");
                    }

                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    {
                        OpenScene();
                        // SceneSerializer serializer(m_ActiveScene);
                        // serializer.Deserialize("assets/scenes/Example.hazel");
                    }
                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    {
                        SaveSenceAs();
                    }
                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            m_SceneHierarchyPanel.OnImGuiRender(m_timestep);
            m_ContentBrowerPanel.OnImGuiRender();

            ImGui::Begin("Stats");
            std::string name = "None";
            if (m_HoveredEntity)
            {
                name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
            }

            ImGui::Text("Hovered Entity: %s", name.c_str());

            auto stats = Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
            m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

            // HZ_INFO("{},{}", viewportOffset.x, viewportOffset.y);
            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
            ImGui::Image((ImTextureRef)(textureID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
                         ImVec2{1, 0});

            if (ImGui::BeginDragDropTarget()) // Accept drag drop payloads
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONNECTOR_DROWSER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    OpenScene(std::filesystem::path(g_AssetPath / path));
                }
                ImGui::EndDragDropTarget();
            }
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                float windowWidth = (float)ImGui::GetWindowWidth();
                float windowHeight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
                // Camera
                // auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
                // const auto& camera = cameraEntity.GetComponent<CameraComponent>();
                // const glm::mat4& cameraProjection = camera.Camera.GetProjection();
                // auto cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

                // Editor camera
                const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

                auto& tc = selectedEntity.GetComponent<TransformComponent>();
                auto transform = tc.GetTransform();

                // Snapping
                bool snap = Input::IsKeyPressed(HazelKey::LeftControl);
                float snapValue = 0.5f;

                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                {
                    snapValue = 45.0f;
                }
                float snapValues[3] = {snapValue, snapValue, snapValue};
                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                                     (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                                     nullptr, snap ? snapValues : nullptr);
                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 translation, rotation, scale;
                    Math::DecomposeTransform(transform, translation, rotation, scale);

                    glm::vec3 deltaRotation = rotation - tc.Rotation;
                    tc.Translation = translation;
                    tc.Rotation += deltaRotation;
                    // HZ_INFO("{},{},{}", tc.Rotation.x, tc.Rotation.y, tc.Rotation.z);
                    tc.Scale = scale;
                }
            }

            ImGui::End();
            ImGui::PopStyleVar();

            UI_Toolbar();
            ImGui::End();
        }
    } //Draw the ImGui interface
    void EditorLayer::UI_Toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 2});
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        auto& color = ImGui::GetStyle().Colors;
        const auto& buttonHovered = color[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
        const auto& buttonActive = color[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
        ImGui::Begin("##toolbar", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        float size = ImGui::GetWindowHeight() - 4.0f;
        Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton(" ", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1)))
        {
            if (m_SceneState == SceneState::Edit)
            {
                OnScenePlay();
            }
            else
            {
                OnSceneStop();
            }
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void EditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;
        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;
        m_ActiveScene->OnRuntimeStop();
        m_ActiveScene = m_EditorScene;
    }


    void EditorLayer::OnEvent(Event& event)
    {
        HZ_PROFILE_FUNCTION();
        m_CameraController.OnEvent(event);
        m_EditorCamera.OnEvent(event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressedEvent));
    } //Respond to events that are distributed by the event system

    bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == HazelKey::MouseLeft)
        {
            if (m_ViewportHovered && !Input::IsKeyPressed(HazelKey::LeftAlt) && !ImGuizmo::IsOver())
            {
                m_SceneHierarchyPanel.SetSeletedEntity(m_HoveredEntity);
            }
        }
        return false;
    }

    bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        if (e.GetRepeatCount() > 0)
            return false;
        bool control = Input::IsKeyPressed(HazelKey::LeftControl) || Input::IsKeyPressed(HazelKey::RightControl);
        bool shift = Input::IsKeyPressed(HazelKey::LeftShift) || Input::IsKeyPressed(HazelKey::RightShift);
        switch (e.GetKeyCode())
        {
        case HazelKey::N:
            {
                if (control)
                    NewSence();
                break;
            }
        case HazelKey::O:
            {
                if (control)
                    OpenScene();
                break;
            }
        case HazelKey::S:
            {
                if (control)
                {
                    if (shift)
                    {
                        SaveSenceAs();
                    }
                    else
                    {
                        SaveSence();
                    }
                }
                break;
            }
        case HazelKey::D:
            {
                if (control)
                    OnDuplicateEntity();
                break;
            }
        case HazelKey::Q:
            {
                m_GizmoType = -1;
                break;
            }
        case HazelKey::W:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                HZ_INFO("{}", "W is pressed");
                break;
            }
        case HazelKey::E:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                HZ_INFO("{}", "R is pressed");

                break;
            }
        case HazelKey::R:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }
        return false;
    }

    void EditorLayer::NewSence()
    {
        HZ_INFO("New Sence");
        // Clear hovered entity to avoid dangling entity handles across scene switches
        m_HoveredEntity = Entity();
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_EditorScenePath = std::filesystem::path();
    }

    void EditorLayer::OpenScene()
    {
        HZ_INFO("OpenScene");
        std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
        if (!filepath.empty())
        {
            // Clear hovered entity when loading a different scene
            OpenScene(filepath);
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& path)
    {
        if (m_SceneState != SceneState::Edit)
        {
            OnSceneStop();
        }
        if (path.extension().string() != ".hazel")
        {
            HZ_WARN("Could not open file '{}' - not a .hazel scene file", path.string());
            return;
        }
        Ref<Scene> newScene = CreateRef<Scene>();
        SceneSerializer serializer(newScene);
        if (serializer.Deserialize(path.string()))
        {
            m_EditorScene = newScene;
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);
            m_ActiveScene = m_EditorScene;
            m_EditorScenePath = path;
        }
    }

    void EditorLayer::SaveSence()
    {
        if(!m_EditorScenePath.empty())
        {
            SerialzeScene(m_ActiveScene,m_EditorScenePath);
        }else
        {
            SaveSenceAs();
        }
    }

    void EditorLayer::SaveSenceAs()
    {
        HZ_INFO("SaveSenceAs");
        std::string filepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
        if (!filepath.empty())
        {
            SerialzeScene(m_ActiveScene,filepath);
            m_EditorScenePath = filepath;
        }
    }
    void EditorLayer::SerialzeScene(Ref<Scene> scene, const std::filesystem::path& path)
    {
        SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(path.string());
    }

    void EditorLayer::OnDuplicateEntity()
    {
        if(m_SceneState != SceneState::Edit)
            return;
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if(selectedEntity)
        {
            m_EditorScene->DuplicateEntity(selectedEntity);
        }
    }
}
