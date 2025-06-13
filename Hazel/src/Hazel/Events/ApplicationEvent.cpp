//
// Created by stang on 25-6-9.
#include <hzpch.h>
#include "ApplicationEvent.h"
#include "Event.h"

namespace Hazel
{
    ///////////////////////WindowResizeEvent/////////////////////////////////////////////////////////////
    WindowResizeEvent::WindowResizeEvent(int width, int height) : m_width(width), m_height(height)
    {
    }

    std::string WindowResizeEvent::ToString() const
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << GetWidth() << " x " << GetHeight();
        return ss.str();
    }

    unsigned int WindowResizeEvent::GetWidth() const { return m_width; }
    unsigned int WindowResizeEvent::GetHeight() const { return m_height; }

    /////////////////WindowCloseEvent///////////////////////////////////////////////////////////////////
    WindowCloseEvent::WindowCloseEvent()
    {
    }

    ////////////////////AppTickEvent/////////////////////////////////////////////////////////////
    AppTickEvent::AppTickEvent()
    {
    }

    //////////////////APPUpdateEvent/////////////////////////////////////////////////////////
    AppUpdateEvent::AppUpdateEvent()
    {
    }

    //////////////////AppRenderEvent////////////////////////////////////////////////////////////////
    AppRenderEvent::AppRenderEvent()
    {
    }
}
