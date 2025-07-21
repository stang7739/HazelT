//
// Created by stang on 25-7-17.
//
#include <Hazel_noEntityPoint.h>
#include <glm/ext/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "SandBox2D.h"
#include "backends/imgui_impl_opengl3.h"
#ifndef EXAMPLELAYER_H
#define EXAMPLELAYER_H



class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer();
    virtual void OnUpdate(Hazel::Timestep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Hazel::Event& event) override;



private:
    Hazel::ShaderLibrary m_ShaderLibrary;
    Hazel::OrthographicCameraController m_CameraController;
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;

    Hazel::Ref<Hazel::Shader> m_Shader;
    Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;
    glm::vec3 m_squarePosition = {0.0f, 0.0f, 0.0f};
    glm::mat4 m_Tranform = glm::mat4(1.0f); // Identity matrix
    float c_RotationSpeed = 90.f;

    float c_CameraMoveSpeed = 1.f;
    float m_squareRotation = 0.0f;

};



#endif //EXAMPLELAYER_H
