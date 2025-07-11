//
// Created by stang on 25-7-3.
//

#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


namespace Hazel
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch(Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HZ_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture>(path);
        }
        HZ_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    }
}
