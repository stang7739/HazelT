//
// Created by stang on 25-7-17.
//
#include"Hazel_noEntityPoint.h"
#include "GameLayer.h"

#include "Random.h"

GameLayer::GameLayer(): Layer("GameLayer")
{
    HZ_PROFILE_FUNCTION();

    auto& window = Hazel::Application::Get().GetWindow();
    CreateCamera(window.GetWidth(), window.GetHeight());
    Hazel::Renderer2D::Init();
    Random::Init();
}


void GameLayer::OnAttach()
{
    HZ_PROFILE_FUNCTION();
    m_Level.Init();
    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);
    // Initialize the camera with orthographic projection
}

void GameLayer::OnDetach()
{
    HZ_PROFILE_FUNCTION();
    Hazel::Renderer2D::Shutdown();
}

void GameLayer::OnUpdate(Hazel::Timestep timestep)
{
    m_Time += timestep;
    if ((int)(m_Time * 10.f) % 8 > 4)
    {
        m_Blink = !m_Blink;
    }
    if (m_Level.IsGameOver())
    {
        m_State = GameState::GameOver;
    }
    const auto& playerPos = m_Level.GetPlayer().GetPosition();
    m_Camera->SetPosition({playerPos.x, playerPos.y, 0.f});

    switch (m_State)
    {
    case GameState::Play:
        {
            m_Level.OnUpdate(timestep);
            break;
        }
    }
    Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Hazel::RenderCommand::Clear();

    Hazel::Renderer2D::BeginScene(*m_Camera);
    m_Level.OnRender();
    Hazel::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Hazel::Event& event)
{
    HZ_PROFILE_FUNCTION();
    Hazel::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(GameLayer::OnWindowResize));
    dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(GameLayer::OnKeyPressed));
    dispatcher.Dispatch<Hazel::MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
    // Handle events here
}

bool GameLayer::OnWindowResize(Hazel::WindowResizeEvent& event)
{
    CreateCamera(event.GetWidth(), event.GetHeight());
    return false;
}

bool GameLayer::OnKeyPressed(Hazel::KeyPressedEvent& event)
{
    return false;
}

bool GameLayer::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event)
{
    if (m_State == GameState::GameOver)
        m_Level.Reset();
    m_State = GameState::Play;
    return false;
}

void GameLayer::OnImGuiRender()
{
    // HZ_PROFILE_FUNCTION();
    // ImGui::Begin(" ");
    // ImGui::Text(" ");
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
    m_Level.OnImGuiRender();
    //UI
    switch (m_State)
    {
    case GameState::Play:
        {
            uint32_t playScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score:") + std::to_string(playScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
            break;
        }
    case GameState::MainMenu:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = Hazel::Application::Get().GetWindow().GetWidth();
            auto height = Hazel::Application::Get().GetWindow().GetHeight();
            // pos.x += width * 0.5f - 300.f;
            // pos.y += 50.f;
            pos.x = width * 0.5f - ImGui::CalcTextSize("Click to Play", nullptr, false, 48.f).x * 0.5f;
            pos.y += 100.f;
            if (m_Blink)
            {
                HZ_CORE_ASSERT(m_Font, "Font load failed!");
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.f, pos, 0xffffffff, "Click to Play");
            }
            break;
        }
    case GameState::GameOver:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = Hazel::Application::Get().GetWindow().GetWidth();
            auto height = Hazel::Application::Get().GetWindow().GetHeight();
            // pos.x += width * 0.5f - 300.f;
            // pos.y += 50.f;
            pos.x = width * 0.5f - ImGui::CalcTextSize("Click to Play", nullptr, false, 48.f).x * 0.5f;
            pos.y += 100.f;
            if (m_Blink)
            {
                HZ_CORE_ASSERT(m_Font, "Font load failed!");
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.f, pos, 0xffffffff, "Click to Play");
            }
            pos.x +=200.f;
            pos.y += 150.f;
            uint32_t playScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score:") + std::to_string(playScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());


        }
    }
    ImGui::End();
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
    float aspectRatio = (float)width / (float)height;

    float camWidth = 8.f;
    float bottom = -camWidth;
    float top = camWidth;
    float left = -camWidth * aspectRatio;
    float right = camWidth * aspectRatio;
    // m_camera = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
    m_Camera = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
}
