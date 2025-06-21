//
// Created by stang on 25-6-14.
//

#include "ImGuiLayer.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "imgui.h"
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "Hazel/Core/Application.h"

namespace Hazel
{
      ImGuiContext* ImGuiLayer::Context_ = nullptr;

    static ImGuiKey HazelKeyToImGuiKey(HazelKey keycode)
    {
        switch (keycode)
        {
        // Mouse Buttons
        case HazelKey::MouseLeft:
            return ImGuiKey_MouseLeft;
        case HazelKey::MouseRight:
            return ImGuiKey_MouseRight;
        case HazelKey::MouseMiddle:
            return ImGuiKey_MouseMiddle;
        case HazelKey::MouseButton4:
            return ImGuiKey_MouseX1;
        case HazelKey::MouseButton5:
            return ImGuiKey_MouseX2;

        // Number Keys (Top row)
        case HazelKey::_0:
            return ImGuiKey_0;
        case HazelKey::_1:
            return ImGuiKey_1;
        case HazelKey::_2:
            return ImGuiKey_2;
        case HazelKey::_3:
            return ImGuiKey_3;
        case HazelKey::_4:
            return ImGuiKey_4;
        case HazelKey::_5:
            return ImGuiKey_5;
        case HazelKey::_6:
            return ImGuiKey_6;
        case HazelKey::_7:
            return ImGuiKey_7;
        case HazelKey::_8:
            return ImGuiKey_8;
        case HazelKey::_9:
            return ImGuiKey_9;

        // Alphabet Keys
        case HazelKey::A:
            return ImGuiKey_A;
        case HazelKey::B:
            return ImGuiKey_B;
        case HazelKey::C:
            return ImGuiKey_C;
        case HazelKey::D:
            return ImGuiKey_D;
        case HazelKey::E:
            return ImGuiKey_E;
        case HazelKey::F:
            return ImGuiKey_F;
        case HazelKey::G:
            return ImGuiKey_G;
        case HazelKey::H:
            return ImGuiKey_H;
        case HazelKey::I:
            return ImGuiKey_I;
        case HazelKey::J:
            return ImGuiKey_J;
        case HazelKey::K:
            return ImGuiKey_K;
        case HazelKey::L:
            return ImGuiKey_L;
        case HazelKey::M:
            return ImGuiKey_M;
        case HazelKey::N:
            return ImGuiKey_N;
        case HazelKey::O:
            return ImGuiKey_O;
        case HazelKey::P:
            return ImGuiKey_P;
        case HazelKey::Q:
            return ImGuiKey_Q;
        case HazelKey::R:
            return ImGuiKey_R;
        case HazelKey::S:
            return ImGuiKey_S;
        case HazelKey::T:
            return ImGuiKey_T;
        case HazelKey::U:
            return ImGuiKey_U;
        case HazelKey::V:
            return ImGuiKey_V;
        case HazelKey::W:
            return ImGuiKey_W;
        case HazelKey::X:
            return ImGuiKey_X;
        case HazelKey::Y:
            return ImGuiKey_Y;
        case HazelKey::Z:
            return ImGuiKey_Z;

        // Function Keys
        case HazelKey::F1:
            return ImGuiKey_F1;
        case HazelKey::F2:
            return ImGuiKey_F2;
        case HazelKey::F3:
            return ImGuiKey_F3;
        case HazelKey::F4:
            return ImGuiKey_F4;
        case HazelKey::F5:
            return ImGuiKey_F5;
        case HazelKey::F6:
            return ImGuiKey_F6;
        case HazelKey::F7:
            return ImGuiKey_F7;
        case HazelKey::F8:
            return ImGuiKey_F8;
        case HazelKey::F9:
            return ImGuiKey_F9;
        case HazelKey::F10:
            return ImGuiKey_F10;
        case HazelKey::F11:
            return ImGuiKey_F11;
        case HazelKey::F12:
            return ImGuiKey_F12;

        // Numpad Keys
        case HazelKey::Numpad0:
            return ImGuiKey_Keypad0;
        case HazelKey::Numpad1:
            return ImGuiKey_Keypad1;
        case HazelKey::Numpad2:
            return ImGuiKey_Keypad2;
        case HazelKey::Numpad3:
            return ImGuiKey_Keypad3;
        case HazelKey::Numpad4:
            return ImGuiKey_Keypad4;
        case HazelKey::Numpad5:
            return ImGuiKey_Keypad5;
        case HazelKey::Numpad6:
            return ImGuiKey_Keypad6;
        case HazelKey::Numpad7:
            return ImGuiKey_Keypad7;
        case HazelKey::Numpad8:
            return ImGuiKey_Keypad8;
        case HazelKey::Numpad9:
            return ImGuiKey_Keypad9;
        case HazelKey::NumpadAdd:
            return ImGuiKey_KeypadAdd;
        case HazelKey::NumpadSubtract:
            return ImGuiKey_KeypadSubtract;
        case HazelKey::NumpadMultiply:
            return ImGuiKey_KeypadMultiply;
        case HazelKey::NumpadDivide:
            return ImGuiKey_KeypadDivide;
        case HazelKey::NumpadEnter:
            return ImGuiKey_KeypadEnter;
        case HazelKey::NumpadDecimal:
            return ImGuiKey_KeypadDecimal;

        // Control Keys
        case HazelKey::Tab:
            return ImGuiKey_Tab;
        case HazelKey::Enter:
            return ImGuiKey_Enter;
        case HazelKey::LeftShift:
            return ImGuiKey_LeftShift;
        case HazelKey::RightShift:
            return ImGuiKey_RightShift;
        case HazelKey::LeftControl:
            return ImGuiKey_LeftCtrl;
        case HazelKey::RightControl:
            return ImGuiKey_RightCtrl;
        case HazelKey::LeftAlt:
            return ImGuiKey_LeftAlt;
        case HazelKey::RightAlt:
            return ImGuiKey_RightAlt;
        case HazelKey::LeftSuper:
            return ImGuiKey_LeftSuper;
        case HazelKey::RightSuper:
            return ImGuiKey_RightSuper;
        case HazelKey::Space:
            return ImGuiKey_Space;
        case HazelKey::CapsLock:
            return ImGuiKey_CapsLock;
        case HazelKey::Escape:
            return ImGuiKey_Escape;
        case HazelKey::Backspace:
            return ImGuiKey_Backspace;
        case HazelKey::PageUp:
            return ImGuiKey_PageUp;
        case HazelKey::PageDown:
            return ImGuiKey_PageDown;
        case HazelKey::Home:
            return ImGuiKey_Home;
        case HazelKey::End:
            return ImGuiKey_End;
        case HazelKey::Insert:
            return ImGuiKey_Insert;
        case HazelKey::Delete:
            return ImGuiKey_Delete;
        case HazelKey::LeftArrow:
            return ImGuiKey_LeftArrow;
        case HazelKey::UpArrow:
            return ImGuiKey_UpArrow;
        case HazelKey::RightArrow:
            return ImGuiKey_RightArrow;
        case HazelKey::DownArrow:
            return ImGuiKey_DownArrow;
        case HazelKey::NumLock:
            return ImGuiKey_NumLock;
        case HazelKey::ScrollLock:
            return ImGuiKey_ScrollLock;

        // Additional Keyboard Keys
        case HazelKey::Apostrophe:
            return ImGuiKey_Apostrophe;
        case HazelKey::Comma:
            return ImGuiKey_Comma;
        case HazelKey::Minus:
            return ImGuiKey_Minus;
        case HazelKey::Period:
            return ImGuiKey_Period;
        case HazelKey::Slash:
            return ImGuiKey_Slash;
        case HazelKey::Semicolon:
            return ImGuiKey_Semicolon;
        case HazelKey::Equal:
            return ImGuiKey_Equal;
        case HazelKey::LeftBracket:
            return ImGuiKey_LeftBracket;
        case HazelKey::Backslash:
            return ImGuiKey_Backslash;
        case HazelKey::RightBracket:
            return ImGuiKey_RightBracket;
        case HazelKey::GraveAccent:
            return ImGuiKey_GraveAccent;

        default:
            HZ_CORE_WARN("HazelKey {} is not mapped in ImGuiKey!", static_cast<int>(keycode));
            return ImGuiKey_None; // Return None for any unrecognized keys
        }
    }

    ImGuiLayer::ImGuiLayer(): Layer("ImGui")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();

         Context_ = ImGui::CreateContext();

        // ImGui::SetCurrentContext(context_);
        // ImGuiContext* ctx = ImGui::GetCurrentContext();
        // HZ_CORE_TRACE("ImGuiLayer::OnAttach Context: {0}", (void*)ctx);
        ImGuiIO& io = ImGui::GetIO();
        // io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        //Change the style of the system mouse cursor based on the internal state of ImGui
        // io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; //Support for ImGui to set mouse position via code
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Enable keyboard navigation support keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; //Enable docking support
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f; //Set the window corner radius to 0
            style.Colors[ImGuiCol_WindowBg].w = 1.0f; //Set the window background color to fully opaque
        }

        Application& app = Application::Get();
        //Bind ImGui to GLFW's input event system, and tell ImGui which GLFW window to use to receive events
        //such as mouse, keyboard, window actions, etc.
        //Specifies the window object on which ImGui's rendering and input events are hung
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    } //Executed when the layer is loaded into the stack
    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    } //Executed when the layer is removed from the stack
    void ImGuiLayer::OnImGuiRender()
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show); //The ImGui Demo window is displayed
    }

    void ImGuiLayer::Begin()
    {
        HZ_CORE_ASSERT(ImGui::GetCurrentContext() != nullptr, "ImGui context is null!");

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        HZ_CORE_TRACE("BackendFlags: {0:x}", ImGui::GetIO().BackendFlags);
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        HZ_CORE_TRACE("ImGuiLayer Context: {0}", (void*)ctx);


    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(),app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }



    // void ImGuiLayer::OnUpdate()
    // {
    //     //io variables can be used to access and set ImGui's inputs
    //     //(e.g., keyboard, mouse) and configuration parameters (e.g., display size, frame rate, etc.).
    //     ImGuiIO& io = ImGui::GetIO();
    //     Application& app = Application::Get();
    //     //Set the display size of ImGui (io. DisplaySize) to match the width
    //     //and height of the app window to ensure that ImGui's interface correctly adapts to the window size
    //     io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
    //
    //     float time = (float)glfwGetTime();
    //     // HZ_INFO("float time: {0}", time);
    //     //Represents the actual time interval (in seconds) between the current frame and the previous frame,
    //     //
    //     //which is used to synchronize ImGui's animations, inputs, etc., with the real frame rate
    //     io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
    //     m_Time = time;
    //
    //     ImGui_ImplOpenGL3_NewFrame();
    //     ImGui::NewFrame();
    //
    //     static bool show = true;
    //     ImGui::ShowDemoWindow(&show); //The ImGui Demo window is displayed
    //
    //     ImGui::Render();
    //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //OpenGL3 draw
    // } //Update logic every frame
    // void ImGuiLayer::OnEvent(Event& event)
    // {
    //     // EventDispatcher dispatcher(event);
    //     // dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
    //     // dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
    //     // dispatcher.Dispatch<MouseMoveEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMoveEvent));
    //     // dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
    //     // dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
    //     // dispatcher.Dispatch<KeyReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
    //     // dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
    //     // dispatcher.Dispatch<KeyTypedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
    // } //Respond to events that are distributed by the event system
//     bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.MouseDown[static_cast<int>(e.GetMouseButton())] = true;
//         return false;
//     }
//
//     bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.MouseDown[static_cast<int>(e.GetMouseButton())] = false;
//         return false;
//     }
//
//     bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.MousePos = ImVec2(e.GetX(), e.GetY());
//         //io.MousePos = ImVec2(0, 0); //For testing purposes, set mouse position to (0, 0)
//
//         return false;
//     }
//
//     bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.MouseWheelH += e.GetOffsetX();
//         io.MouseWheel += e.GetOffsetY();
//         return false;
//     }
//
//     bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.AddKeyEvent(HazelKeyToImGuiKey(e.GetKeyCode()), true);
//         UpdateModifierKeys();
//
//         return false;
//     }
//
//     bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.AddKeyEvent(HazelKeyToImGuiKey(e.GetKeyCode()), false);
//         UpdateModifierKeys();
//         return false;
//     }
//
//     bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         io.DisplaySize = ImVec2(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
//         io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f); // Assuming no DPI scaling for simplicity
//         glViewport(0, 0, e.GetWidth(), e.GetHeight());
//         return false;
//     }
//
//     bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
//     {
//         ImGuiIO& io = ImGui::GetIO();
//         int Keycodepoint = e.GetCodePoint();
//         if (Keycodepoint > 0 && Keycodepoint < 0x10000) // Check if the code point is valid
//         {
//             io.AddInputCharacter(static_cast<unsigned short>(Keycodepoint));
//         }
//
//         return false;
//     }
//
//     void ImGuiLayer::UpdateModifierKeys()
//     {
//         ImGuiIO& io = ImGui::GetIO();
//
//         GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
//
//         io.AddKeyEvent(ImGuiKey_ModCtrl,
//                        glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
//                        glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
//         io.AddKeyEvent(ImGuiKey_ModShift,
//                        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
//                        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
//         io.AddKeyEvent(ImGuiKey_ModAlt,
//                        glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
//                        glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);
//         io.AddKeyEvent(ImGuiKey_ModSuper,
//                        glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS ||
//                        glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS);
//     }
}
