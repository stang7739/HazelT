//
// Created by stang on 25-7-11.
//

#ifndef SANDBOX2D_H
#define SANDBOX2D_H
#include "Hazel_noEntityPoint.h"



namespace Hazel
{
    class Texture2D;
    class Shader;
    class VertexArray;
}

class SandBox2D : public  Hazel::Layer{
public:
    SandBox2D() ;
    virtual ~SandBox2D() = default;
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
    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};



#endif //SANDBOX2D_H
