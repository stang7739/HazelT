//
// Created by stang on 25-6-7.
//

#include <hzpch.h>
#include <glfw/glfw3.h>

#include "Applicaton.h"
#include <Hazel/Events/ApplicationEvent.h>
#include<Hazel/Core/Log.h>



namespace Hazel
{
    //This macro is used to bind the member function fn of the Application class to the current object (this)
    //and reserve a placeholder _1 to indicate that a parameter needs to be passed in when calling.
#define BIND_EVENT_FN(fn) std::bind(&Applicaton::fn, this, std::placeholders::_1)
    Applicaton::Applicaton()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }
    Applicaton::~Applicaton()
    {

    }
    void Applicaton::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        HZ_CORE_TRACE("{0}", e);

    }
    void Applicaton::run()
    {
        WindowResizeEvent e(1260,720);

        HZ_TRACE(e);

        while (m_Running)
        {
            glClearColor(1,0,1,1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
    bool Applicaton::OnWindowClose(Event& e)
    {
        m_Running = false;
        return true;
    }


}
