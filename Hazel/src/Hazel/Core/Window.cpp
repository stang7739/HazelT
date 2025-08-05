//
// Created by stang on 25-6-10.
//
#include <hzpch.h>
#include "Hazel/Core/Window.h"

#ifdef HZ_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#elif defined(HZ_PLATFORM_LINUX)
    #include "Platform/Windows/WindowsWindow.h"  // Using same implementation for now
#endif

namespace Hazel
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
#if defined(HZ_PLATFORM_WINDOWS) || defined(HZ_PLATFORM_LINUX)
        return CreateScope<WindowsWindow>(props);
#else
        HZ_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }

}