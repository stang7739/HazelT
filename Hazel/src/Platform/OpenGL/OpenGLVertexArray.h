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

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

    private:
        uint32_t m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}


#endif //OPENGLVERTEXARRAY_H
