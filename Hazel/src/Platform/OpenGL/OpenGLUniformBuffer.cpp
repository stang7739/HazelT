//
// Created by stang on 25-9-20.
//
#include "hzpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Hazel
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
    {
        HZ_PROFILE_FUNCTION();
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }
    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        HZ_PROFILE_FUNCTION();
        glDeleteBuffers(1, &m_RendererID);
    }
    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset )
    {
        glNamedBufferSubData(m_RendererID, offset, size, data);
    }
}
