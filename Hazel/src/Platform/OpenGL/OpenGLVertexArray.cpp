//
// Created by stang on 25-6-25.
//

#include "OpenGLVertexArray.h"
#include <glad/glad.h>

#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case Hazel::ShaderDataType::Float: return GL_FLOAT;
        case Hazel::ShaderDataType::Float2: return GL_FLOAT;
        case Hazel::ShaderDataType::Float3: return GL_FLOAT;
        case Hazel::ShaderDataType::Float4: return GL_FLOAT;
        case Hazel::ShaderDataType::Int: return GL_INT;
        case Hazel::ShaderDataType::Int2: return GL_INT;
        case Hazel::ShaderDataType::Int3: return GL_INT;
        case Hazel::ShaderDataType::Int4: return GL_INT;
        case Hazel::ShaderDataType::Bool: return GL_BOOL;
        }
        HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1,&m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),"VertexBuffer has no layout!");
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for(const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetCompoentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)(uintptr_t)element.Offset
                );
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        // HZ_CORE_TRACE("VertexArray::SetIndexBuffer called, ptr = {0}", (void*)indexBuffer.get());

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }

    const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
    {
        return m_VertexBuffers;
    }

    const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
    {
        return m_IndexBuffer;
    }
}
