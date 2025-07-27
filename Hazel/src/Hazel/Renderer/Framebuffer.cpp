//
// Created by stang on 25-7-27.
//

#include "Framebuffer.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hazel
{


    Ref<Framebuffer> Framebuffer::Create(FramebufferSpecification& spec)
    {
         switch(RendererAPI::GetAPI())
         {
         case RendererAPI::API::None:
             HZ_CORE_ASSERT(false,"RendererAPI::None is not supported!");
         case RendererAPI::API::OpenGL:
             return CreateRef<OpenGLFramebuffer>(spec);
         }
        HZ_ERROR("None of the RendererAPI is supported!");
        return nullptr;

    }
}
