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
#include <Hazel/Renderer/Shader.h>

#include "Timestep.h"
#include "GLFW/glfw3.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
    //This macro is used to bind the member function fn of the Application class to the current object (this)
    //and reserve a placeholder _1 to indicate that a parameter needs to be passed in when calling.

    Application* Application::s_Instance = nullptr;


    Application::Application()
    {
        HZ_CORE_ASSERT(!s_Instance, "Application already exit")
        HZ_CORE_INFO("Application created");

        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
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

        HZ_TRACE(e);

        while (m_Running)
        {
            //The number of seconds from the start of GLFW initialization to the current moment
            float time = (float)glfwGetTime();
            Timestep timestep =  time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate(timestep);
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

    bool Application::OnWindowClose(Event& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnKeyPressed(Event& e)
    {
        KeyPressedEvent& event = (KeyPressedEvent&)e;
        if (event.GetKeyCode() == HazelKey::Escape)
        {
            m_Running = false;
            return true;
        }
        return false;
    }
}
