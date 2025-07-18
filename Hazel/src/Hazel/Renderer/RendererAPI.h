//
// Created by stang on 25-6-12.
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Hazel/Core/Base.h"

namespace Hazel
{
    class HAZEL_API RendererAPI {

        public:
        enum class API
        {
            None = 0,
            OpenGL = 1
        };
        virtual ~RendererAPI() = default;
        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x,uint32_t y,uint32_t width,uint32_t height)=0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        // virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t indexCount = 0) = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray =0) = 0;
        virtual void DrawLine(const Ref<VertexArray>& vertexArray,uint32_t indexCount ) = 0;

        virtual void SetLineWidth(float width) = 0;

        static API GetAPI(){return s_API;}
        static Scope<RendererAPI> Create();

        private:
        static API s_API;
    };
}



#endif //RENDERERAPI_H
