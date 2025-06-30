//
// Created by stang on 25-6-27.
//

#include "RenderCommand.h"

#include "Platform/OpenGL/Renderer_OpenGLRenererAPI.h"

namespace Hazel
{
   RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRenererAPI;
}
