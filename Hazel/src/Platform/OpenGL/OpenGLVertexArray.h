//
// Created by stang on 25-6-25.
//

#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H
#include "Hazel/Renderer/VertexArray.h"


namespace Hazel
{
    class IndexBuffer;

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray() override;
        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override;

    private:
        uint32_t m_RendererID;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}


#endif //OPENGLVERTEXARRAY_H
