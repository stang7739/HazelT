//
// Created by stang on 25-7-11.
//

#include "SandBox2D.h"

#include <imgui.h>


#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Renderer/Renderer2D.h"
#include <chrono>


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

SandBox2D::SandBox2D(): Layer("Sandbox2D"), m_CameraController(1260.f / 720.f, true),m_SquareColor(1,1,1,1.f)
// Initialize the camera with orthographic projection
{
    HZ_PROFILE_FUNCTION();
    Hazel::Renderer2D::Init();
}

void SandBox2D::OnAttach()
{
    HZ_PROFILE_FUNCTION();
    m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
    m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
} //Executed when the layer is loaded into the stack
void SandBox2D::OnDetach()
{
} //Executed when the layer is removed from the stack
void SandBox2D::OnUpdate(Hazel::Timestep timestep)
{
    m_Rotation = m_Rotation <= 180 ?  (m_Rotation+= 1 * m_Speed)  : 0;
    HZ_PROFILE_FUNCTION();
    // PROFILE_SCOPE("SandBox2D::OnUpdate");
    Hazel::Renderer2D::ResetStats();
    {
        HZ_PROFILE_SCOPE("CameraController::OnUpdate");
        // PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(timestep);
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Hazel::RenderCommand::Clear();
    }

    {
        HZ_PROFILE_SCOPE("Renderer Prep");
        // Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
    }

    {
        // HZ_INFO("{}",m_Rotation);
        HZ_PROFILE_SCOPE("Renderer Draw");
        for(int i =0;i< m_Count;i++)
        {
            float x = (i % 10) * 0.4f - 1.f;
            float y = (i / 10) * 0.4f - 1.f;
            glm::vec4 color = {static_cast<float>(i % 10) / 10.f, static_cast<float>(i % 5) / 5.f, static_cast<float>(i % 3) / 3.f, 1.0f};
            Hazel::Renderer2D::DrawQuad({x, y, 0.0f}, {0.2f, 0.2f}, m_SquareColor);
        }
       Hazel::Renderer2D::DrawQuad({0.f, 0.f, -0.1f}, {20.0f, 20.0f}, m_CheckerboardTexture, 10.f, m_SquareColor);

        // Hazel::Renderer2D::DrawQuad({0.5f, -0.5f, 0.0f}, {0.8f, 0.8f}, m_CheckerboardTexture, 1.f, {1, 1, 1, 1});

        // Hazel::Renderer2D::DrawQuad({0.6f, -0.6f, 0.0f}, {0.8f, 0.8f}, m_CheckerboardTexture);
        Hazel::Renderer2D::DrawRotatedQuad({0.5f, -0.5f, 0.0f}, {0.8f, 0.8f}, m_Rotation, m_CheckerboardTexture);

        Hazel::Renderer2D::EndScene();
        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -(m_speedsquare); y < m_speedsquare; y += m_speedsquare / 10.f)
        {
            for (float x = -m_speedsquare; x < m_speedsquare; x += m_speedsquare / 10.f)
            {
                glm::vec4 color = {(x + 5.f) / 10.f, 0.4f, (y + 5.f) / 10.f, 1.0f};
                Hazel::Renderer2D::DrawQuad({x, y, 0.0f}, {0.4f, 0.4f}, color);
            }
        }
        Hazel::Renderer2D::EndScene();
    }
} //Update logic every frame
void SandBox2D::OnEvent(Hazel::Event& event)
{
    HZ_PROFILE_FUNCTION();
    m_CameraController.OnEvent(event);
} //Respond to events that are distributed by the event system
void SandBox2D::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();

    ImGui::Begin("SandBox2D");
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
    auto stats = Hazel::Renderer2D::GetStats();
    ImGui::Text("DrawCall: %d", stats.DrawCalls);
    ImGui::Text("QuadCount: %d", stats.QuadCount);
    ImGui::Text("VertexCount: %d", stats.GetTotalVertexCount());
    ImGui::Text("IndexCount: %d", stats.GetTotalIndexCount());
    m_ProfileResults.clear();
    ImGui::End();
} //Draw the ImGui interface
