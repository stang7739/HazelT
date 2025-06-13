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
    Applicaton::Applicaton()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }
    Applicaton::~Applicaton()
    {

    }

    void Applicaton::run()
    {
        WindowResizeEvent e(1260,720);

        HZ_TRACE(e.ToString());

        while (m_Running)
        {
            glClearColor(1,0,1,1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }



}
