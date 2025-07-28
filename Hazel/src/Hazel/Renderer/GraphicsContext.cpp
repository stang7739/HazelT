//
// Created by stang on 25-7-28.
//
#include <hzpch.h>
#include "GraphicsContext.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }
        // This function should create and return a specific graphics context implementation
        // based on the platform (e.g., OpenGL, Vulkan, etc.). For now, we will return a nullptr.
        HZ_CORE_ASSERT(false, "GraphicsContext::Create not implemented for this platform!");
        return nullptr;
    }
}
