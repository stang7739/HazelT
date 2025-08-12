//
// Created by stang on 25-7-11.
//

#include "SandBox2D.h"

#include <imgui.h>
#include <cstdint>

#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Renderer/Renderer2D.h"
#include <chrono>

#include "Hazel/Renderer/Framebuffer.h"

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

SandBox2D::SandBox2D(): Layer("Sandbox2D"), m_CameraController(1260.f / 720.f, true), m_SquareColor(1, 1, 1, 1.f)
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
    m_TextureStairs = Hazel::SubTexture2D::CreateFromCoords(m_CheckerboardTexture, {0, 0}, {1, 1}, {0.2f, 0.2f});
    Hazel::FramebufferSpecification fbspec;
    fbspec.Width = 1280;
    fbspec.Height = 720;
    m_Framebuffer = Hazel::Framebuffer::Create(fbspec);
} //Executed when the layer is loaded into the stack
void SandBox2D::OnDetach()
{
} //Executed when the layer is removed from the stack
void SandBox2D::OnUpdate(Hazel::Timestep timestep)
{
    m_Rotation = m_Rotation <= 180 ? (m_Rotation += 1 * m_Speed) : 0;
    HZ_PROFILE_FUNCTION();
    // PROFILE_SCOPE("SandBox2D::OnUpdate");
    Hazel::Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    {
        HZ_PROFILE_SCOPE("CameraController::OnUpdate");
        // PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(timestep);
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Hazel::RenderCommand::Clear();
        
        // Execute the render graph with early-Z optimization
        // This will run the DefaultDepthRenderSlot with early-Z enabled
        static bool loggedOnce = false;
        if (!loggedOnce) {
            HZ_INFO("Executing render graph with early-Z optimization enabled");
            loggedOnce = true;
        }
        Hazel::Renderer::ExecuteRenderGraph();
    }

    {
        HZ_PROFILE_SCOPE("Renderer Prep");
        // Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
    }

    {
        // HZ_INFO("{}",m_Rotation);
        HZ_PROFILE_SCOPE("Renderer Draw");
        for (int i = 0; i < m_Count; i++)
        {
            float x = (i % 10) * 0.4f - 1.f;
            float y = (i / 10) * 0.4f - 1.f;
            glm::vec4 color = {
                static_cast<float>(i % 10) / 10.f, static_cast<float>(i % 5) / 5.f, static_cast<float>(i % 3) / 3.f,
                1.0f
            };
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
        m_Framebuffer->Unbind();
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

                if (ImGui::MenuItem("Exit")) Hazel::Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");

        auto stats = Hazel::Renderer2D::GetStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::Separator();
        ImGui::Text("Render Graph (LWRP) Stats:");
        auto& renderGraph = Hazel::Renderer::GetRenderGraph();
        auto depthSlot = renderGraph.GetNode("DefaultDepthRenderSlot");
        if (depthSlot) {
            ImGui::Text("Early-Z Optimization: Enabled");
            ImGui::Text("Depth Prepass: Active");
            ImGui::Text("Render Pipeline: LWRP with Early-Z");
        } else {
            ImGui::Text("Early-Z Optimization: Not Available");
        }

        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(TextureIDToImGuiHandle(textureID), ImVec2{1280.0f, 720.0f});

        textureID = m_Framebuffer->GetDepthAttachmentRendererID();
        ImGui::Image(TextureIDToImGuiHandle(textureID), ImVec2{1280.0f, 720.0f});
        ImGui::End();

        ImGui::End();
    }
    else
    {
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
    }
} //Draw the ImGui interface
