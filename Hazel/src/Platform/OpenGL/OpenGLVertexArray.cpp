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
        glCreateVertexArrays(1, &m_RendererID);
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
        HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout!");
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
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
                    break;
                }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribIPointer(index, element.GetCompoentCount(),
                                           ShaderDataTypeToOpenGLBaseType(element.Type), layout.GetStride(),
                                           (const void*)(uintptr_t)element.Offset);
                    index++;
                    break;
                }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                {
                    uint8_t count = element.GetCompoentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(index);
                        glVertexAttribPointer(index,
                                              count,
                                              ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (const void*)(uintptr_t)(element.Offset + sizeof(float) * count * i)
                        );
                        glVertexAttribDivisor(index, 1);
                        index++;
                    }
                    break;
                }
            }
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
