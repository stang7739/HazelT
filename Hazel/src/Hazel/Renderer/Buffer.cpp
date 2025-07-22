//
// Created by stang on 25-6-24.
//

#include "Buffer.h"

#include "Hazel/Renderer/Renderer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel
{
    /////////////////////////////////////////////////////////////////
    //VertexBuffer///////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    Ref<VertexBuffer> VertexBuffer::Create( uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return  std::make_shared<OpenGLVertexBuffer>( size);
        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return  std::make_shared<OpenGLVertexBuffer>(vertices, size);
        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    /////////////////////////////////////////////////////////////////
    //IndexBuffer///////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////



    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return  std::make_shared< OpenGLIndexBuffer>(indices, size);
        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
