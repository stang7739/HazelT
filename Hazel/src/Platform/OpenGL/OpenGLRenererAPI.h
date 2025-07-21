//
// Created by stang on 25-6-12.
//

#ifndef RENDERER_OPENGLRENERERAPI_H
#define RENDERER_OPENGLRENERERAPI_H
#include "Hazel/Renderer/Renderer.h"


namespace Hazel
{
    class OpenGLRenererAPI : public RendererAPI{
        public:
        virtual void Init() override;
        virtual void SetViewport(uint32_t x,uint32_t y,uint32_t width,uint32_t height)override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        // virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t indexCount = 0) override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        virtual void DrawLine(const Ref<VertexArray>& vertexArray,uint32_t indexCount ) override;

        virtual void SetLineWidth(float width) override;
    };
}



#endif //RENDERER_OPENGLRENERERAPI_H
