//
// Created by stang on 25-7-11.
//

#ifndef EDITORLAYER_H
#define EDITORLAYER_H
#include "Hazel_noEntityPoint.h"
#include "Hazel/Renderer/EditorCamera.h"
#include "Hazel/Scene/Entity.h"
#include "Panels/ContentBrowerPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace entt
{
    enum class entity : std::uint32_t;
}
namespace Hazel
{
    class Framebuffer;
    class Texture2D;
    class Shader;
    class VertexArray;
    class Scene;

    class EditorLayer : public  Layer{
    public:
        EditorLayer() ;
        virtual ~EditorLayer() override = default ;
        virtual void OnAttach() override; //Executed when the layer is loaded into the stack
        virtual void OnDetach() override;//Executed when the layer is removed from the stack
        virtual void OnUpdate(Timestep timestep)override;//Update logic every frame
        virtual void OnEvent(Event& event)override;//Respond to events that are distributed by the event system
        virtual void OnImGuiRender()override;//Draw the ImGui interface
    private:
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        void NewScene();
        void OpenScene();
        void OpenScene(const std::filesystem::path& path);
        void SaveSceneAs();
        void SaveScene();
        void SerialzeScene(Ref<Scene> scene,const std::filesystem::path& path);
        void OnScenePlay();
        void OnSceneStop();
        void OnSceneSimulate();
        void OnDuplicateEntity();
        void UI_Toolbar();
        void OnOverlayRender();

        struct ProfileResult
        {
            const char* Name;
            float Time;
        };

        enum class SceneState
        {
            Edit =0,
            Play =1,
            Simulate =2,
        };
        SceneState m_SceneState = SceneState::Edit;
        OrthographicCameraController m_CameraController;
        SceneHierarchyPanel m_SceneHierarchyPanel;
        ContentBrowerPanel m_ContentBrowerPanel;
        Timestep m_timestep;
        EditorCamera m_EditorCamera;

        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_BlueShader;
        Ref<Texture2D> m_CheckerboardTexture, m_ChernoLogoTexture,m_IconPlay,m_IconStop,m_IconSimulate;
        Ref<SubTexture2D> m_TextureStairs;
        Ref<Scene> m_ActiveScene;
        Ref<Scene> m_EditorScene;
        std::filesystem::path m_EditorScenePath;
        Ref<Framebuffer> m_Framebuffer;

        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;
        Entity m_HoveredEntity;

        glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2  m_ViewportBounds[2];
        std::vector<ProfileResult> m_ProfileResults;
        float m_Rotation = 0.0f;
        float m_Speed = 0.5f;
        int m_Count = 1;
        int m_speedsquare = 5.f;
        bool m_ViewportFocused = false, m_ViewportHovered = false;
        bool m_PrimaryCamera = true;
        bool m_ShowPhysicsColliders = false;
        int m_GizmoType = -1;

    };
}

#endif //EDITORLAYER_H
