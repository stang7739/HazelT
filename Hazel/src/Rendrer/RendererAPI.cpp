//
// Created by stang on 25-6-12.
//

#include "RendererAPI.h"

#include "Hazel/Core/Core.h"
#include "OpenGL/Renderer_OpenGLRenererAPI.h"

namespace Hazel
{

#ifdef OPENGL
     RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif

     Scope<RendererAPI> RendererAPI::Create()
     {
          switch (s_API)
          {
               case RendererAPI::API::OpenGL:
                    return CreateScope<OpenGLRenererAPI>();
               case RendererAPI::API::None:
                    HZ_CORE_ASSERT(false, "RendererAPI::None is not supported!");
                    return nullptr;
               default:
                    HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
               return nullptr;
          }
          HZ_CORE_ASSERT(false, "Unknown RendererAPI!");


     }
}
