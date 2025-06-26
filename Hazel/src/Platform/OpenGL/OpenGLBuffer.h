//
// Created by stang on 25-6-24.
//

#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H
#include <Hazel/Renderer/Buffer.h>

namespace Hazel
{
    //Forward claims allow pointers or references to be declared, but cannot be used to inherit or instantiate objects
    // class VertexBuffer;
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const BufferLayout& GetLayout() const override;
        virtual void SetLayout(const BufferLayout& layout) override;

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
        ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const override; // Returns the number of indices

    private:
        uint32_t m_RendererID;
        uint32_t m_Count; // Number of indices
    };
}


#endif //OPENGLBUFFER_H
