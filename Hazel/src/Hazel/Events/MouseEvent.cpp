//
// Created by stang on 25-6-9.
//
#include <hzpch.h>
#include "MouseEvent.h"

namespace Hazel
{
    ////////////////MouseMoveEvent///////////////////
    MouseMoveEvent::MouseMoveEvent(double x, double y) : m_MouseX(x), m_MouseY(y)
    {
    }

    std::string MouseMoveEvent::ToString() const
    {
        return std::format("MouseMoveEvent: {0}, {1}", m_MouseX, m_MouseY);
    }

    int MouseMoveEvent::GetCategoryFlags() const { return EventCategoryInput | EventCategoryMouse; }
    ///////////////MouseScrolled///////////////////////
    MouseScrollEvent::MouseScrollEvent(double offsetX, double offsetY) : m_OffsetX(offsetX), m_OffsetY(offsetY)
    {
    }

    inline float MouseScrollEvent::GetOffsetX() const
    {
        return m_OffsetX;
    }

    inline float MouseScrollEvent::GetOffsetY() const
    {
        return m_OffsetY;
    }

    std::string MouseScrollEvent::ToString() const
    {
        return std::format("MouseScrollEvent: {0}, {1}", m_OffsetX, m_OffsetY);
    }

    int MouseScrollEvent::GetCategoryFlags() const { return EventCategoryInput | EventCategoryMouse; }
    ///////////////////////MouseButtonPressed/////////
    MouseButtonEvent::MouseButtonEvent(HazelKey Mousebutton) : m_MouseButton(Mousebutton)
    {
    }

    inline HazelKey MouseButtonEvent::GetMouseButton() const
    {
        return m_MouseButton;
    }

    int MouseButtonEvent::GetCategoryFlags() const
    {
        return EventCategoryInput | EventCategoryMouse;
    }

    ////////////////////MouseButtonPressed/////////
    MouseButtonPressedEvent::MouseButtonPressedEvent(HazelKey Mousebutton): MouseButtonEvent(Mousebutton)
    {
    }

    std::string MouseButtonPressedEvent::ToString() const
    {
        return std::format("MouseButtonPressedEvent: {0}", m_MouseButton);
    }

    ////////////////MouseButtonReleased//////////
    MouseButtonReleasedEvent::MouseButtonReleasedEvent(HazelKey Mousebutton): MouseButtonEvent(Mousebutton)
    {
    }

    std::string MouseButtonReleasedEvent::ToString() const
    {
        return std::format("MouseButtonReleasedWvent : {0}",m_MouseButton);
    }
}
