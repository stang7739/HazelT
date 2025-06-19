 //
 // Created by stang on 25-6-18.
 //
 #include "hzpch.h"
 #include<glad/glad.h>
 #include "WindowsInput.h"

 #include <Hazel/Core/Application.h>

 #include "GLFW/glfw3.h"

 namespace Hazel
 {
     Input* Input::s_Instance = new WindowsInput();
     // Input* Input::s_Instance = nullptr;

     static int HazelKeyToGlfwKey(HazelKey keycode)
     {
         switch (keycode)
         {
         // Mouse Buttons
         case HazelKey::MouseLeft:
             return GLFW_MOUSE_BUTTON_LEFT;
         case HazelKey::MouseRight:
             return GLFW_MOUSE_BUTTON_RIGHT;
         case HazelKey::MouseMiddle:
             return GLFW_MOUSE_BUTTON_MIDDLE;
         case HazelKey::MouseButton4:
             return GLFW_MOUSE_BUTTON_4;
         case HazelKey::MouseButton5:
             return GLFW_MOUSE_BUTTON_5;

         // Number Keys (Top row)
         case HazelKey::_0:
             return GLFW_KEY_0;
         case HazelKey::_1:
             return GLFW_KEY_1;
         case HazelKey::_2:
             return GLFW_KEY_2;
         case HazelKey::_3:
             return GLFW_KEY_3;
         case HazelKey::_4:
             return GLFW_KEY_4;
         case HazelKey::_5:
             return GLFW_KEY_5;
         case HazelKey::_6:
             return GLFW_KEY_6;
         case HazelKey::_7:
             return GLFW_KEY_7;
         case HazelKey::_8:
             return GLFW_KEY_8;
         case HazelKey::_9:
             return GLFW_KEY_9;

         // Alphabet Keys
         case HazelKey::A:
             return GLFW_KEY_A;
         case HazelKey::B:
             return GLFW_KEY_B;
         case HazelKey::C:
             return GLFW_KEY_C;
         case HazelKey::D:
             return GLFW_KEY_D;
         case HazelKey::E:
             return GLFW_KEY_E;
         case HazelKey::F:
             return GLFW_KEY_F;
         case HazelKey::G:
             return GLFW_KEY_G;
         case HazelKey::H:
             return GLFW_KEY_H;
         case HazelKey::I:
             return GLFW_KEY_I;
         case HazelKey::J:
             return GLFW_KEY_J;
         case HazelKey::K:
             return GLFW_KEY_K;
         case HazelKey::L:
             return GLFW_KEY_L;
         case HazelKey::M:
             return GLFW_KEY_M;
         case HazelKey::N:
             return GLFW_KEY_N;
         case HazelKey::O:
             return GLFW_KEY_O;
         case HazelKey::P:
             return GLFW_KEY_P;
         case HazelKey::Q:
             return GLFW_KEY_Q;
         case HazelKey::R:
             return GLFW_KEY_R;
         case HazelKey::S:
             return GLFW_KEY_S;
         case HazelKey::T:
             return GLFW_KEY_T;
         case HazelKey::U:
             return GLFW_KEY_U;
         case HazelKey::V:
             return GLFW_KEY_V;
         case HazelKey::W:
             return GLFW_KEY_W;
         case HazelKey::X:
             return GLFW_KEY_X;
         case HazelKey::Y:
             return GLFW_KEY_Y;
         case HazelKey::Z:
             return GLFW_KEY_Z;

         // Function Keys
         case HazelKey::F1:
             return GLFW_KEY_F1;
         case HazelKey::F2:
             return GLFW_KEY_F2;
         case HazelKey::F3:
             return GLFW_KEY_F3;
         case HazelKey::F4:
             return GLFW_KEY_F4;
         case HazelKey::F5:
             return GLFW_KEY_F5;
         case HazelKey::F6:
             return GLFW_KEY_F6;
         case HazelKey::F7:
             return GLFW_KEY_F7;
         case HazelKey::F8:
             return GLFW_KEY_F8;
         case HazelKey::F9:
             return GLFW_KEY_F9;
         case HazelKey::F10:
             return GLFW_KEY_F10;
         case HazelKey::F11:
             return GLFW_KEY_F11;
         case HazelKey::F12:
             return GLFW_KEY_F12;

         // Numpad Keys
         case HazelKey::Numpad0:
             return GLFW_KEY_KP_0;
         case HazelKey::Numpad1:
             return GLFW_KEY_KP_1;
         case HazelKey::Numpad2:
             return GLFW_KEY_KP_2;
         case HazelKey::Numpad3:
             return GLFW_KEY_KP_3;
         case HazelKey::Numpad4:
             return GLFW_KEY_KP_4;
         case HazelKey::Numpad5:
             return GLFW_KEY_KP_5;
         case HazelKey::Numpad6:
             return GLFW_KEY_KP_6;
         case HazelKey::Numpad7:
             return GLFW_KEY_KP_7;
         case HazelKey::Numpad8:
             return GLFW_KEY_KP_8;
         case HazelKey::Numpad9:
             return GLFW_KEY_KP_9;
         case HazelKey::NumpadAdd:
             return GLFW_KEY_KP_ADD;
         case HazelKey::NumpadSubtract:
             return GLFW_KEY_KP_SUBTRACT;
         case HazelKey::NumpadMultiply:
             return GLFW_KEY_KP_MULTIPLY;
         case HazelKey::NumpadDivide:
             return GLFW_KEY_KP_DIVIDE;
         case HazelKey::NumpadEnter:
             return GLFW_KEY_KP_ENTER;
         case HazelKey::NumpadDecimal:
             return GLFW_KEY_KP_DECIMAL;

         // Control Keys
         case HazelKey::Tab:
             return GLFW_KEY_TAB;
         case HazelKey::Enter:
             return GLFW_KEY_ENTER;
         case HazelKey::LeftShift:
             return GLFW_KEY_LEFT_SHIFT;
         case HazelKey::RightShift:
             return GLFW_KEY_RIGHT_SHIFT;
         case HazelKey::LeftControl:
             return GLFW_KEY_LEFT_CONTROL;
         case HazelKey::RightControl:
             return GLFW_KEY_RIGHT_CONTROL;
         case HazelKey::LeftAlt:
             return GLFW_KEY_LEFT_ALT;
         case HazelKey::RightAlt:
             return GLFW_KEY_RIGHT_ALT;
         case HazelKey::LeftSuper:
             return GLFW_KEY_LEFT_SUPER;
         case HazelKey::RightSuper:
             return GLFW_KEY_RIGHT_SUPER;
         case HazelKey::Space:
             return GLFW_KEY_SPACE;
         case HazelKey::CapsLock:
             return GLFW_KEY_CAPS_LOCK;
         case HazelKey::Escape:
             return GLFW_KEY_ESCAPE;
         case HazelKey::Backspace:
             return GLFW_KEY_BACKSPACE;
         case HazelKey::PageUp:
             return GLFW_KEY_PAGE_UP;
         case HazelKey::PageDown:
             return GLFW_KEY_PAGE_DOWN;
         case HazelKey::Home:
             return GLFW_KEY_HOME;
         case HazelKey::End:
             return GLFW_KEY_END;
         case HazelKey::Insert:
             return GLFW_KEY_INSERT;
         case HazelKey::Delete:
             return GLFW_KEY_DELETE;
         case HazelKey::LeftArrow:
             return GLFW_KEY_LEFT;
         case HazelKey::UpArrow:
             return GLFW_KEY_UP;
         case HazelKey::RightArrow:
             return GLFW_KEY_RIGHT;
         case HazelKey::DownArrow:
             return GLFW_KEY_DOWN;
         case HazelKey::NumLock:
             return GLFW_KEY_NUM_LOCK;
         case HazelKey::ScrollLock:
             return GLFW_KEY_SCROLL_LOCK;

         // Additional Keyboard Keys
         case HazelKey::Apostrophe:
             return GLFW_KEY_APOSTROPHE;
         case HazelKey::Comma:
             return GLFW_KEY_COMMA;
         case HazelKey::Minus:
             return GLFW_KEY_MINUS;
         case HazelKey::Period:
             return GLFW_KEY_PERIOD;
         case HazelKey::Slash:
             return GLFW_KEY_SLASH;
         case HazelKey::Semicolon:
             return GLFW_KEY_SEMICOLON;
         case HazelKey::Equal:
             return GLFW_KEY_EQUAL;
         case HazelKey::LeftBracket:
             return GLFW_KEY_LEFT_BRACKET;
         case HazelKey::Backslash:
             return GLFW_KEY_BACKSLASH;
         case HazelKey::RightBracket:
             return GLFW_KEY_RIGHT_BRACKET;
         case HazelKey::GraveAccent:
             return GLFW_KEY_GRAVE_ACCENT;

         default:
             HZ_CORE_WARN("HazelKey {} is not mapped in GLFW!", static_cast<int>(keycode));
             return -1; // Return invalid GLFW key
         }
     }

     bool WindowsInput::IsKeyPressedImpl(HazelKey Keycode)
     {
         auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
         if (!window)
         {

                 HZ_CORE_ERROR("Window is null in GetMousePositionImpl!");

             return false;
         }
         auto state = glfwGetKey(window, HazelKeyToGlfwKey(Keycode));
         return state == GLFW_PRESS || state == GLFW_REPEAT;
     }

     bool WindowsInput::IsMouseButtonPressedImpl(HazelKey Button)
     {
         auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
         if (!window)
         {
             HZ_CORE_ERROR("Window is null in GetMousePositionImpl!");
             return false;
         }
         auto state = glfwGetMouseButton(window, HazelKeyToGlfwKey(Button));
         return state == GLFW_PRESS;
     }

     std::pair<float, float> WindowsInput::GetMousePositionImpl()
     {
         auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
         double xPos, yPos;
         if (!window)
         {
             HZ_CORE_ERROR("Window is null in GetMousePositionImpl!");
             return {0.0f, 0.0f};
         }
         glfwGetCursorPos(window, &xPos, &yPos);
         return {(float)xPos, (float)yPos};
     }

     float WindowsInput::GetMouseXImpl()
     {
         auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
         auto [x, y] = GetMousePosition();
         if (!window) return 0.0f;
         return x;
     }

     float WindowsInput::GetMouseYImpl()
     {
         auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
         auto [x, y] = GetMousePosition();
         if (!window) return 0.0f;

         return y;
     }
 }
