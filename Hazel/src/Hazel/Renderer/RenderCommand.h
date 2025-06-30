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
        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }
        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }
    private:
        static RendererAPI* s_RendererAPI;
    };
}



#endif //RENDERCOMMAND_H
