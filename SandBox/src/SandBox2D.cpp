//
// Created by stang on 25-7-11.
//

#include "SandBox2D.h"

#include <imgui.h>


#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Renderer/Renderer2D.h"

SandBox2D::SandBox2D():Layer("Sandbox2D"),m_CameraController(1260.f / 720.f, true) // Initialize the camera with orthographic projection
{
    Hazel::Renderer2D::Init();


}
void SandBox2D::OnAttach()
{
    m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
    m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
} //Executed when the layer is loaded into the stack
void SandBox2D::OnDetach()
{
} //Executed when the layer is removed from the stack
void SandBox2D::OnUpdate(Hazel::Timestep timestep)
{
    m_CameraController.OnUpdate(timestep);
    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
    // Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    // Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
    Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, { 0.8f, 0.8f}, m_ChernoLogoTexture);
    Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, { 0.8f, 0.8f}, m_CheckerboardTexture);
    Hazel::Renderer2D::EndScene();

} //Update logic every frame
void SandBox2D::OnEvent(Hazel::Event& event)
{
} //Respond to events that are distributed by the event system
void SandBox2D::OnImGuiRender()
{
    ImGui::Begin("SandBox2D");
    ImGui::ColorEdit4("squre Color",glm::value_ptr(m_SquareColor));
    ImGui::End();
} //Draw the ImGui interface
