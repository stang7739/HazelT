//
// Created by stang on 25-6-27.
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H
#include "RendererAPI.h"


namespace Hazel
{
    class VertexArray;

    class HAZEL_API RenderCommand {
    public:
        inline static void init()
        {
            s_RendererAPI->Init();
        }

        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t count = 0)
        {
            s_RendererAPI->DrawIndexed(vertexArray,count);
        }
        inline static void SetViewport(uint32_t x, uint32_t y,uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x,y,width,height);
        }
        
        // Early-Z and depth prepass support
        inline static void BeginDepthPrepass()
        {
            s_RendererAPI->BeginDepthPrepass();
        }
        
        inline static void EndDepthPrepass()
        {
            s_RendererAPI->EndDepthPrepass();
        }
        
        inline static void SetDepthFunction(uint32_t func)
        {
            s_RendererAPI->SetDepthFunction(func);
        }
    private:
        static RendererAPI* s_RendererAPI;
    };
}



#endif //RENDERCOMMAND_H
