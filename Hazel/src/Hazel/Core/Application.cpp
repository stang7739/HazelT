//
// Created by stang on 25-6-7.
//

#include <hzpch.h>
#include <glad/glad.h>

#include "Application.h"
#include <Hazel/Events/ApplicationEvent.h>
#include<Hazel/Core/Log.h>

#include "imgui_internal.h"
#include "Input.h"
#include "Hazel/Events/KeyEvent.h"


#include "Timestep.h"
#include "Hazel/Utils/PlatformUtils.h"

#include "Hazel/Renderer/Renderer.h"


namespace Hazel
{
    //This macro is used to bind the member function fn of the Application class to the current object (this)
    //and reserve a placeholder _1 to indicate that a parameter needs to be passed in when calling.

    Application* Application::s_Instance = nullptr;


    Application::Application(const ApplicationSpecification& specification):m_Specification(specification)
    {
        HZ_CORE_ASSERT(!s_Instance, "Application already exit");
        HZ_CORE_INFO("Application created");

        s_Instance = this;
        if (!m_Specification.WorkingDirectory.empty())
        {
            std::error_code ec;
            std::filesystem::current_path(m_Specification.WorkingDirectory, ec);
            if (ec)
                HZ_CORE_WARN("Failed to set working directory to '{}': {}", m_Specification.WorkingDirectory, ec.message());
            else
                HZ_CORE_INFO("Working directory set to '{}'", std::filesystem::current_path().string());
        }

        // Always create a window; use application Name as the window title
        m_Window = Window::Create(WindowProps(m_Specification.Name));
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
        Renderer::init();
        m_ImGuiLayer = new ImGuiLayer{};
        PushOverlayer(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlayer(Layer* layer)
    {
        m_LayerStack.PushOverLayer(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(Application::OnKeyPressed));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)break;
        }
        // HZ_CORE_TRACE("{0}", e);
    }

    void Application::run()
    {
        WindowResizeEvent e(1260, 720);

        HZ_TRACE("{}",e);

        while (m_Running)
        {
            //The number of seconds from the start of GLFW initialization to the current moment
            float time = Time::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate(timestep);
                }
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                HZ_CORE_ASSERT(layer, "Layer is nullptr!");
                // ImGuiContext* ctx = ImGui::GetCurrentContext();
                // HZ_CORE_TRACE("Application ImGui Context: {0}", (void*)ctx);
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
            // // auto [x,y] = Input::GetMousePosition();
            // // HZ_CORE_TRACE("{0}, {1}", x, y);
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
    void Application::Close()
    {
        m_Running = false;
    }
    bool Application::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.GetKeyCode() == HazelKey::Escape)
        {
            m_Running = false;
            return true;
        }
        return false;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (static_cast<WindowResizeEvent&>(e).GetHeight() == 0 ||
            static_cast<WindowResizeEvent&>(e).GetWidth() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        // int fbWidth, fbHeight;
        // glfwGetFramebufferSize((GLFWwindow*)m_Window->GetNativeWindow(), &fbWidth, &fbHeight);
        // glViewport(0, 0, fbWidth, fbHeight);
        // HZ_INFO("""Viewport set to x: {0}, y: {1}, width: {2}, height: {3}", 0, 0, fbWidth, fbHeight);

        Renderer::OnWindowResize(static_cast<WindowResizeEvent&>(e).GetWidth(),
                                 static_cast<WindowResizeEvent&>(e).GetHeight());
        return false;
    }
}
