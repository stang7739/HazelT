//
// Created by stang on 25-6-12.
//

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#include "Hazel/Core/Base.h"


namespace Hazel
{
    class IndexBuffer;
    class VertexBuffer;

    class HAZEL_API VertexArray {
    public:
        virtual ~VertexArray() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };
}



#endif //VERTEXARRAY_H
