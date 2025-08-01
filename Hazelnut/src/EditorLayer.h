//
// Created by stang on 25-7-11.
//

#ifndef EDITORLAYER_H
#define EDITORLAYER_H
#include "Hazel_noEntityPoint.h"
#include "Hazel/Scene/Entity.h"


namespace entt
{
    enum class entity : std::uint32_t;
}

namespace Hazel
{
    class Scene;
}

namespace Hazel
{
    class Framebuffer;



    class Texture2D;
    class Shader;
    class VertexArray;


    class EditorLayer : public  Layer{
    public:
        EditorLayer() ;
        virtual ~EditorLayer() = default;
        virtual void OnAttach() override; //Executed when the layer is loaded into the stack
        virtual void OnDetach() override;//Executed when the layer is removed from the stack
        virtual void OnUpdate(Timestep timestep)override;//Update logic every frame
        virtual void OnEvent(Event& event)override;//Respond to events that are distributed by the event system
        virtual void OnImGuiRender()override;//Draw the ImGui interface
    private:
        OrthographicCameraController m_CameraController;
        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_BlueShader;

        Ref<Texture2D> m_CheckerboardTexture, m_ChernoLogoTexture;
        Ref<SubTexture2D> m_TextureStairs;
        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
        struct ProfileResult
        {
            const char* Name;
            float Time;
        };
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        std::vector<ProfileResult> m_ProfileResults;
        float m_Rotation = 0.0f;
        float m_Speed = 0.5f;
        int m_Count = 1;
        int m_speedsquare = 5.f;
        Ref<Framebuffer> m_Framebuffer;
        bool m_ViewportFocused = false, m_ViewportHovered = false;



    };
}

#endif //EDITORLAYER_H
