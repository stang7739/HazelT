//
// Created by stang on 25-6-10.
//
#include <hzpch.h>
#include "WindowsWindow.h"
#include<Hazel/Core/Log.h>
#include<Rendrer/Renderer.h>

namespace Hazel
{
    int s_GLFWInitialized = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        HZ_CORE_INFO("Create Window {0} ({1}, {2}) ", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            HZ_CORE_ASSERT(success, " Could not initialize GLFW");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }
        {
#ifdef HZ_DEBUG
            //When an error occurs when calling an OpenGL function, debugging information is actively generated
            if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
            m_window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
            glfwMakeContextCurrent(m_window);
            glfwSetWindowUserPointer(m_window, &m_Data);
            SetVSync(true);
        }
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    uint32_t WindowsWindow::GetHeight() const { return m_Data.Height; }
    uint32_t WindowsWindow::GetWidth() const { return m_Data.Width; }

    // Window attributes
    void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
    {
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    void* WindowsWindow::GetNativeWindow() const
    {
        return m_window;
    }
}
