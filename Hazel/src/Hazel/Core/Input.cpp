//
// Created by stang on 25-6-18.
//

#include "Input.h"

#include "Platform/Windows/WindowsInput.h"


namespace Hazel
{


    Ref<Input> Input::Create()
    {
#ifdef HZ_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
#elif
        return nullptr;
#endif
    }
}
