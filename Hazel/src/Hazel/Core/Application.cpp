//
// Created by stang on 25-6-7.
//

#include <hzpch.h>
#include <glad/glad.h>

#include "Application.h"
#include <Hazel/Events/ApplicationEvent.h>
#include<Hazel/Core/Log.h>

#include "Input.h"
#include "Hazel/Events/KeyEvent.h"


namespace Hazel
{
    //This macro is used to bind the member function fn of the Application class to the current object (this)
    //and reserve a placeholder _1 to indicate that a parameter needs to be passed in when calling.

    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        HZ_CORE_ASSERT(!s_Instance,"Application already exit")
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
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
        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if(e.Handled)break;
        }
        HZ_CORE_TRACE("{0}", e);

    }
    void Application::run()
    {
        WindowResizeEvent e(1260,720);

        HZ_TRACE(e);

        while (m_Running)
        {
            glClearColor(1,0,1,1);
            glClear(GL_COLOR_BUFFER_BIT);
            for(auto layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            auto [x,y] = Input::GetMousePosition();
            HZ_CORE_TRACE("{0}, {1}", x, y);
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
        if(event.GetKeyCode() == HazelKey::Escape)
        {
            m_Running = false;
            return true;
        }
        return false;
    }


}
