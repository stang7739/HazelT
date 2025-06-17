//
// Created by stang on 25-6-10.
//
#include <hzpch.h>
#include <glad/glad.h>
#include "WindowsWindow.h"
#include<Hazel/Core/Log.h>
#include<Rendrer/Renderer.h>

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
    static  bool s_GLFWInitialized = false;
    HazelKey GlfwKeyToHazelKey(int key);
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
            int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            HZ_CORE_ASSERT(status, "Could not initialize GLAD");
            glfwSetWindowUserPointer(m_window, &m_Data);
            SetVSync(true);
        }
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            WindowResizeEvent event(width, height);
            if (data.EventCallback)
                data.EventCallback(event);
            else
                HZ_CORE_WARN("EventCallback is null! (WindowResizeEvent)");
        });
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            auto keycode = GlfwKeyToHazelKey(key);
            switch (action)
            {
                case GLFW_RELEASE:
                    {
                        KeyReleasedEvent event(keycode);
                        data.EventCallback(event);
                        break;
                    }
                case GLFW_PRESS:
                    {
                        KeyPressedEvent event(keycode,0);
                    data.EventCallback(event);
                        break;
                    }
                case GLFW_REPEAT:
                    {
                        KeyPressedEvent event(keycode,1);
                    data.EventCallback(event);
                        break;
                    }
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(codepoint);
            data.EventCallback(event);
        });
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            auto keycode = GlfwKeyToHazelKey(button);
            switch (action)
            {
                case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent event(keycode);
                        data.EventCallback(event);
                        break;
                    }
            case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(keycode);
                        data.EventCallback(event);
                        break;
                    }
            }
        });
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event(xoffset, yoffset);
            data.EventCallback(event);
        });
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMoveEvent event(xpos, ypos);
            data.EventCallback(event);
        });
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
        m_Data.EventCallback = callback;
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

    HazelKey GlfwKeyToHazelKey(int keycode) {
    switch (keycode) {
        // Mouse Buttons
        case GLFW_MOUSE_BUTTON_LEFT:
            return HazelKey::MouseLeft;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return HazelKey::MouseRight;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return HazelKey::MouseMiddle;
        case GLFW_MOUSE_BUTTON_4:
            return HazelKey::MouseButton4;
        case GLFW_MOUSE_BUTTON_5:
            return HazelKey::MouseButton5;

        // Number keys (top row)
        case GLFW_KEY_0:
            return HazelKey::_0;
        case GLFW_KEY_1:
            return HazelKey::_1;
        case GLFW_KEY_2:
            return HazelKey::_2;
        case GLFW_KEY_3:
            return HazelKey::_3;
        case GLFW_KEY_4:
            return HazelKey::_4;
        case GLFW_KEY_5:
            return HazelKey::_5;
        case GLFW_KEY_6:
            return HazelKey::_6;
        case GLFW_KEY_7:
            return HazelKey::_7;
        case GLFW_KEY_8:
            return HazelKey::_8;
        case GLFW_KEY_9:
            return HazelKey::_9;

        // Alphabet keys
        case GLFW_KEY_A:
            return HazelKey::A;
        case GLFW_KEY_B:
            return HazelKey::B;
        case GLFW_KEY_C:
            return HazelKey::C;
        case GLFW_KEY_D:
            return HazelKey::D;
        case GLFW_KEY_E:
            return HazelKey::E;
        case GLFW_KEY_F:
            return HazelKey::F;
        case GLFW_KEY_G:
            return HazelKey::G;
        case GLFW_KEY_H:
            return HazelKey::H;
        case GLFW_KEY_I:
            return HazelKey::I;
        case GLFW_KEY_J:
            return HazelKey::J;
        case GLFW_KEY_K:
            return HazelKey::K;
        case GLFW_KEY_L:
            return HazelKey::L;
        case GLFW_KEY_M:
            return HazelKey::M;
        case GLFW_KEY_N:
            return HazelKey::N;
        case GLFW_KEY_O:
            return HazelKey::O;
        case GLFW_KEY_P:
            return HazelKey::P;
        case GLFW_KEY_Q:
            return HazelKey::Q;
        case GLFW_KEY_R:
            return HazelKey::R;
        case GLFW_KEY_S:
            return HazelKey::S;
        case GLFW_KEY_T:
            return HazelKey::T;
        case GLFW_KEY_U:
            return HazelKey::U;
        case GLFW_KEY_V:
            return HazelKey::V;
        case GLFW_KEY_W:
            return HazelKey::W;
        case GLFW_KEY_X:
            return HazelKey::X;
        case GLFW_KEY_Y:
            return HazelKey::Y;
        case GLFW_KEY_Z:
            return HazelKey::Z;

        // Function keys
        case GLFW_KEY_F1:
            return HazelKey::F1;
        case GLFW_KEY_F2:
            return HazelKey::F2;
        case GLFW_KEY_F3:
            return HazelKey::F3;
        case GLFW_KEY_F4:
            return HazelKey::F4;
        case GLFW_KEY_F5:
            return HazelKey::F5;
        case GLFW_KEY_F6:
            return HazelKey::F6;
        case GLFW_KEY_F7:
            return HazelKey::F7;
        case GLFW_KEY_F8:
            return HazelKey::F8;
        case GLFW_KEY_F9:
            return HazelKey::F9;
        case GLFW_KEY_F10:
            return HazelKey::F10;
        case GLFW_KEY_F11:
            return HazelKey::F11;
        case GLFW_KEY_F12:
            return HazelKey::F12;

        // Numpad keys
        case GLFW_KEY_KP_0:
            return HazelKey::Numpad0;
        case GLFW_KEY_KP_1:
            return HazelKey::Numpad1;
        case GLFW_KEY_KP_2:
            return HazelKey::Numpad2;
        case GLFW_KEY_KP_3:
            return HazelKey::Numpad3;
        case GLFW_KEY_KP_4:
            return HazelKey::Numpad4;
        case GLFW_KEY_KP_5:
            return HazelKey::Numpad5;
        case GLFW_KEY_KP_6:
            return HazelKey::Numpad6;
        case GLFW_KEY_KP_7:
            return HazelKey::Numpad7;
        case GLFW_KEY_KP_8:
            return HazelKey::Numpad8;
        case GLFW_KEY_KP_9:
            return HazelKey::Numpad9;
        case GLFW_KEY_KP_ADD:
            return HazelKey::NumpadAdd;
        case GLFW_KEY_KP_SUBTRACT:
            return HazelKey::NumpadSubtract;
        case GLFW_KEY_KP_MULTIPLY:
            return HazelKey::NumpadMultiply;
        case GLFW_KEY_KP_DIVIDE:
            return HazelKey::NumpadDivide;
        case GLFW_KEY_KP_ENTER:
            return HazelKey::NumpadEnter;
        case GLFW_KEY_KP_DECIMAL:
            return HazelKey::NumpadDecimal;

        // Control Keys
        case GLFW_KEY_TAB:
            return HazelKey::Tab;
        case GLFW_KEY_ENTER:
            return HazelKey::Enter;
        case GLFW_KEY_LEFT_SHIFT:
            return HazelKey::LeftShift;
        case GLFW_KEY_RIGHT_SHIFT:
            return HazelKey::RightShift;
        case GLFW_KEY_LEFT_CONTROL:
            return HazelKey::LeftControl;
        case GLFW_KEY_RIGHT_CONTROL:
            return HazelKey::RightControl;
        case GLFW_KEY_LEFT_ALT:
            return HazelKey::LeftAlt;
        case GLFW_KEY_RIGHT_ALT:
            return HazelKey::RightAlt;
        case GLFW_KEY_LEFT_SUPER:
            return HazelKey::LeftSuper;
        case GLFW_KEY_RIGHT_SUPER:
            return HazelKey::RightSuper;
        case GLFW_KEY_SPACE:
            return HazelKey::Space;
        case GLFW_KEY_CAPS_LOCK:
            return HazelKey::CapsLock;
        case GLFW_KEY_ESCAPE:
            return HazelKey::Escape;
        case GLFW_KEY_BACKSPACE:
            return HazelKey::Backspace;
        case GLFW_KEY_PAGE_UP:
            return HazelKey::PageUp;
        case GLFW_KEY_PAGE_DOWN:
            return HazelKey::PageDown;
        case GLFW_KEY_HOME:
            return HazelKey::Home;
        case GLFW_KEY_END:
            return HazelKey::End;
        case GLFW_KEY_INSERT:
            return HazelKey::Insert;
        case GLFW_KEY_DELETE:
            return HazelKey::Delete;
        case GLFW_KEY_LEFT:
            return HazelKey::LeftArrow;
        case GLFW_KEY_UP:
            return HazelKey::UpArrow;
        case GLFW_KEY_RIGHT:
            return HazelKey::RightArrow;
        case GLFW_KEY_DOWN:
            return HazelKey::DownArrow;
        case GLFW_KEY_NUM_LOCK:
            return HazelKey::NumLock;
        case GLFW_KEY_SCROLL_LOCK:
            return HazelKey::ScrollLock;

        // Additional Keyboard Keys
        case GLFW_KEY_APOSTROPHE:
            return HazelKey::Apostrophe;
        case GLFW_KEY_COMMA:
            return HazelKey::Comma;
        case GLFW_KEY_MINUS:
            return HazelKey::Minus;
        case GLFW_KEY_PERIOD:
            return HazelKey::Period;
        case GLFW_KEY_SLASH:
            return HazelKey::Slash;
        case GLFW_KEY_SEMICOLON:
            return HazelKey::Semicolon;
        case GLFW_KEY_EQUAL:
            return HazelKey::Equal;
        case GLFW_KEY_LEFT_BRACKET:
            return HazelKey::LeftBracket;
        case GLFW_KEY_BACKSLASH:
            return HazelKey::Backslash;
        case GLFW_KEY_RIGHT_BRACKET:
            return HazelKey::RightBracket;
        case GLFW_KEY_GRAVE_ACCENT:
            return HazelKey::GraveAccent;

        default:
            HZ_CORE_WARN("Keycode {} in GLFW is not mapped in HazelKey!", keycode);
            return HazelKey::None; // Return None for any unrecognized keys
    }
}
    
}
