//
// Created by stang on 25-6-9.
//
#include <hzpch.h>
#include "KeyEvent.h"

namespace Hazel
{
    ///////////////////KeyEvent////////////////////////////////////////
    KeyEvent::KeyEvent(HazelKey keyCode) : m_keyCode(keyCode)
    {
    }

    HazelKey KeyEvent::GetKeyCode() const
    {
        return m_keyCode;
    }
    int KeyEvent::GetCategoryFlags() const { return EventCategoryInput | EventCategoryKeyBoard; }

    //////////////////////////
    KeyPressedEvent::KeyPressedEvent(HazelKey keycode, bool isRepeat) : KeyEvent(keycode), m_IsRepeat(isRepeat)
    {
    }

    bool KeyPressedEvent::IsRepeat() const
    {
        return m_IsRepeat;
    }

    std::string KeyPressedEvent::ToString() const {
        return std::format("KeyPressedEvent: {0} ({1})", m_keyCode, m_IsRepeat);
    }

    ///////////////////////////////
    KeyReleasedEvent::KeyReleasedEvent(HazelKey keyCode): KeyEvent(keyCode)
    {
    }

    std::string KeyReleasedEvent::ToString() const
    {
        return std::format("KeyReleasedEvent:{}", m_keyCode);
    }
    /////////////////////////
    KeyTypedEvent::KeyTypedEvent( unsigned int  codepoint) :m_codePoint(codepoint){}
     unsigned int KeyTypedEvent::GetCodePoint() const{ return m_codePoint; }
    std::string KeyTypedEvent::ToString() const
    {
        return std::format("type:{}",std::string(1,static_cast<char32_t>(m_codePoint)));
    }


    int KeyTypedEvent::GetCategoryFlags() const { return EventCategoryInput | EventCategoryKeyBoard; }
}
