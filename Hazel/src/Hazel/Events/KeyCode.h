//
// Created by stang on 25-6-9.
//
#include <format>
#ifndef KEYCODE_H
#define KEYCODE_H

enum class HazelKey : int {
    None = -1,

    // Mouse Buttons
    MouseLeft = 0, // Left Mouse Button
    MouseRight,    // Right Mouse Button
    MouseMiddle,   // Middle Mouse Button (usually scroll wheel button)
    MouseButton4,  // Additional Mouse Button 4 (e.g., forward navigation)
    MouseButton5,  // Additional Mouse Button 5 (e.g., backward navigation)

    // Number Keys (Top row, above QWERTY)
    _0 = 48, // 0
    _1,      // 1
    _2,      // 2
    _3,      // 3
    _4,      // 4
    _5,      // 5
    _6,      // 6
    _7,      // 7
    _8,      // 8
    _9,      // 9

    // Alphabet Keys
    A = 65, // A
    B,      // B
    C,      // C
    D,      // D
    E,      // E
    F,      // F
    G,      // G
    H,      // H
    I,      // I
    J,      // J
    K,      // K
    L,      // L
    M,      // M
    N,      // N
    O,      // O
    P,      // P
    Q,      // Q
    R,      // R
    S,      // S
    T,      // T
    U,      // U
    V,      // V
    W,      // W
    X,      // X
    Y,      // Y
    Z,      // Z

    // Function Keys
    F1,  // F1
    F2,  // F2
    F3,  // F3
    F4,  // F4
    F5,  // F5
    F6,  // F6
    F7,  // F7
    F8,  // F8
    F9,  // F9
    F10, // F10
    F11, // F11
    F12, // F12

    // Numpad Keys
    Numpad0,        // Numpad 0
    Numpad1,        // Numpad 1
    Numpad2,        // Numpad 2
    Numpad3,        // Numpad 3
    Numpad4,        // Numpad 4
    Numpad5,        // Numpad 5
    Numpad6,        // Numpad 6
    Numpad7,        // Numpad 7
    Numpad8,        // Numpad 8
    Numpad9,        // Numpad 9
    NumpadAdd,      // Numpad +
    NumpadSubtract, // Numpad -
    NumpadMultiply, // Numpad *
    NumpadDivide,   // Numpad /
    NumpadEnter,    // Numpad Enter
    NumpadDecimal,  // Numpad .

    // Control Keys
    Tab,          // Tab
    Enter,        // Enter / Return
    LeftShift,    // Left Shift
    RightShift,   // Right Shift
    LeftControl,  // Left Control (Ctrl)
    RightControl, // Right Control (Ctrl)
    LeftAlt,      // Left Alt
    RightAlt,     // Right Alt
    LeftSuper,    // Left Super (Windows in windows platform)
    RightSuper,   // Right Super (Windows in windows platform)
    Space,        // Spacebar
    CapsLock,     // Caps Lock
    Escape,       // Escape
    Backspace,    // Backspace
    PageUp,       // Page Up
    PageDown,     // Page Down
    Home,         // Home
    End,          // End
    Insert,       // Insert
    Delete,       // Delete
    LeftArrow,    // Left Arrow
    UpArrow,      // Up Arrow
    RightArrow,   // Right Arrow
    DownArrow,    // Down Arrow
    NumLock,      // Num Lock
    ScrollLock,   // Scroll Lock

    // Additional Keyboard Keys
    Apostrophe,   // ' (Apostrophe)
    Comma,        // , (Comma)
    Minus,        // - (Minus)
    Period,       // . (Period)
    Slash,        // / (Slash)
    Semicolon,    // ; (Semicolon)
    Equal,        // = (Equal Sign)
    LeftBracket,  // [ (Left Bracket)
    Backslash,    // \ (Backslash)
    RightBracket, // ] (Right Bracket)
    GraveAccent,  // ` (Grave Accent / Tilde)
};

enum class iGeMouseButton : int { Left = 0, Right, Middle, Button4, Button5 };

 template<>
struct std::formatter<HazelKey> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    auto format(const HazelKey& key, format_context& ctx) const {
        std::string keyName;
        switch (key) {
            // Mouse Buttons
            case HazelKey::MouseLeft:
                keyName = "MouseLeft";
                break;
            case HazelKey::MouseRight:
                keyName = "MouseRight";
                break;
            case HazelKey::MouseMiddle:
                keyName = "MouseMiddle";
                break;
            case HazelKey::MouseButton4:
                keyName = "MouseButton4";
                break;
            case HazelKey::MouseButton5:
                keyName = "MouseButton5";
                break;

            // Number Keys
            case HazelKey::_0:
                keyName = "0";
                break;
            case HazelKey::_1:
                keyName = "1";
                break;
            case HazelKey::_2:
                keyName = "2";
                break;
            case HazelKey::_3:
                keyName = "3";
                break;
            case HazelKey::_4:
                keyName = "4";
                break;
            case HazelKey::_5:
                keyName = "5";
                break;
            case HazelKey::_6:
                keyName = "6";
                break;
            case HazelKey::_7:
                keyName = "7";
                break;
            case HazelKey::_8:
                keyName = "8";
                break;
            case HazelKey::_9:
                keyName = "9";
                break;

            // Alphabet Keys
            case HazelKey::A:
                keyName = "A";
                break;
            case HazelKey::B:
                keyName = "B";
                break;
            case HazelKey::C:
                keyName = "C";
                break;
            case HazelKey::D:
                keyName = "D";
                break;
            case HazelKey::E:
                keyName = "E";
                break;
            case HazelKey::F:
                keyName = "F";
                break;
            case HazelKey::G:
                keyName = "G";
                break;
            case HazelKey::H:
                keyName = "H";
                break;
            case HazelKey::I:
                keyName = "I";
                break;
            case HazelKey::J:
                keyName = "J";
                break;
            case HazelKey::K:
                keyName = "K";
                break;
            case HazelKey::L:
                keyName = "L";
                break;
            case HazelKey::M:
                keyName = "M";
                break;
            case HazelKey::N:
                keyName = "N";
                break;
            case HazelKey::O:
                keyName = "O";
                break;
            case HazelKey::P:
                keyName = "P";
                break;
            case HazelKey::Q:
                keyName = "Q";
                break;
            case HazelKey::R:
                keyName = "R";
                break;
            case HazelKey::S:
                keyName = "S";
                break;
            case HazelKey::T:
                keyName = "T";
                break;
            case HazelKey::U:
                keyName = "U";
                break;
            case HazelKey::V:
                keyName = "V";
                break;
            case HazelKey::W:
                keyName = "W";
                break;
            case HazelKey::X:
                keyName = "X";
                break;
            case HazelKey::Y:
                keyName = "Y";
                break;
            case HazelKey::Z:
                keyName = "Z";
                break;

            // Function Keys
            case HazelKey::F1:
                keyName = "F1";
                break;
            case HazelKey::F2:
                keyName = "F2";
                break;
            case HazelKey::F3:
                keyName = "F3";
                break;
            case HazelKey::F4:
                keyName = "F4";
                break;
            case HazelKey::F5:
                keyName = "F5";
                break;
            case HazelKey::F6:
                keyName = "F6";
                break;
            case HazelKey::F7:
                keyName = "F7";
                break;
            case HazelKey::F8:
                keyName = "F8";
                break;
            case HazelKey::F9:
                keyName = "F9";
                break;
            case HazelKey::F10:
                keyName = "F10";
                break;
            case HazelKey::F11:
                keyName = "F11";
                break;
            case HazelKey::F12:
                keyName = "F12";
                break;

            // Numpad Keys
            case HazelKey::Numpad0:
                keyName = "Numpad0";
                break;
            case HazelKey::Numpad1:
                keyName = "Numpad1";
                break;
            case HazelKey::Numpad2:
                keyName = "Numpad2";
                break;
            case HazelKey::Numpad3:
                keyName = "Numpad3";
                break;
            case HazelKey::Numpad4:
                keyName = "Numpad4";
                break;
            case HazelKey::Numpad5:
                keyName = "Numpad5";
                break;
            case HazelKey::Numpad6:
                keyName = "Numpad6";
                break;
            case HazelKey::Numpad7:
                keyName = "Numpad7";
                break;
            case HazelKey::Numpad8:
                keyName = "Numpad8";
                break;
            case HazelKey::Numpad9:
                keyName = "Numpad9";
                break;
            case HazelKey::NumpadAdd:
                keyName = "NumpadAdd";
                break;
            case HazelKey::NumpadSubtract:
                keyName = "NumpadSubtract";
                break;
            case HazelKey::NumpadMultiply:
                keyName = "NumpadMultiply";
                break;
            case HazelKey::NumpadDivide:
                keyName = "NumpadDivide";
                break;
            case HazelKey::NumpadEnter:
                keyName = "NumpadEnter";
                break;
            case HazelKey::NumpadDecimal:
                keyName = "NumpadDecimal";
                break;

            // Control Keys
            case HazelKey::Tab:
                keyName = "Tab";
                break;
            case HazelKey::Enter:
                keyName = "Enter";
                break;
            case HazelKey::LeftShift:
                keyName = "LeftShift";
                break;
            case HazelKey::RightShift:
                keyName = "RightShift";
                break;
            case HazelKey::LeftControl:
                keyName = "LeftControl";
                break;
            case HazelKey::RightControl:
                keyName = "RightControl";
                break;
            case HazelKey::LeftAlt:
                keyName = "LeftAlt";
                break;
            case HazelKey::RightAlt:
                keyName = "RightAlt";
                break;
            case HazelKey::LeftSuper:
                keyName = "LeftSuper";
                break;
            case HazelKey::RightSuper:
                keyName = "RightSuper";
                break;
            case HazelKey::Space:
                keyName = "Space";
                break;
            case HazelKey::CapsLock:
                keyName = "CapsLock";
                break;
            case HazelKey::Escape:
                keyName = "Escape";
                break;
            case HazelKey::Backspace:
                keyName = "Backspace";
                break;
            case HazelKey::PageUp:
                keyName = "PageUp";
                break;
            case HazelKey::PageDown:
                keyName = "PageDown";
                break;
            case HazelKey::Home:
                keyName = "Home";
                break;
            case HazelKey::End:
                keyName = "End";
                break;
            case HazelKey::Insert:
                keyName = "Insert";
                break;
            case HazelKey::Delete:
                keyName = "Delete";
                break;
            case HazelKey::LeftArrow:
                keyName = "LeftArrow";
                break;
            case HazelKey::UpArrow:
                keyName = "UpArrow";
                break;
            case HazelKey::RightArrow:
                keyName = "RightArrow";
                break;
            case HazelKey::DownArrow:
                keyName = "DownArrow";
                break;
            case HazelKey::NumLock:
                keyName = "NumLock";
                break;
            case HazelKey::ScrollLock:
                keyName = "ScrollLock";
                break;

            // Additional Keyboard Keys
            case HazelKey::Apostrophe:
                keyName = "Apostrophe";
                break;
            case HazelKey::Comma:
                keyName = "Comma";
                break;
            case HazelKey::Minus:
                keyName = "Minus";
                break;
            case HazelKey::Period:
                keyName = "Period";
                break;
            case HazelKey::Slash:
                keyName = "Slash";
                break;
            case HazelKey::Semicolon:
                keyName = "Semicolon";
                break;
            case HazelKey::Equal:
                keyName = "Equal";
                break;
            case HazelKey::LeftBracket:
                keyName = "LeftBracket";
                break;
            case HazelKey::Backslash:
                keyName = "Backslash";
                break;
            case HazelKey::RightBracket:
                keyName = "RightBracket";
                break;
            case HazelKey::GraveAccent:
                keyName = "GraveAccent";
                break;

            default:
                keyName = "UnknownKey";
                break;
        }

        return format_to(ctx.out(), "{}", keyName);
    }
};

#endif //KEYCODE_H
