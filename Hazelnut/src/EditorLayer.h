//
// Created by stang on 25-7-11.
//

#ifndef EDITORLAYER_H
#define EDITORLAYER_H
#include "Hazel_noEntityPoint.h"


namespace Hazel
{
    class Framebuffer;
}

namespace Hazel
{
    class Texture2D;
    class Shader;
    class VertexArray;
}

class EditorLayer : public  Hazel::Layer{
public:
    EditorLayer() ;
    virtual ~EditorLayer() = default;
    virtual void OnAttach() override; //Executed when the layer is loaded into the stack
    virtual void OnDetach() override;//Executed when the layer is removed from the stack
    virtual void OnUpdate(Hazel::Timestep timestep)override;//Update logic every frame
    virtual void OnEvent(Hazel::Event& event)override;//Respond to events that are distributed by the event system
    virtual void OnImGuiRender()override;//Draw the ImGui interface
private:
    Hazel::OrthographicCameraController m_CameraController;
    Hazel::Ref<Hazel::VertexArray> m_SquareVA;
    Hazel::Ref<Hazel::Shader> m_BlueShader;

    Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture, m_ChernoLogoTexture;
    Hazel::Ref<Hazel::SubTexture2D> m_TextureStairs;
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
    Hazel::Ref<Hazel::Framebuffer> m_Framebuffer;
    bool m_ViewportFocused = false, m_ViewportHovered = false;


};



#endif //EDITORLAYER_H
