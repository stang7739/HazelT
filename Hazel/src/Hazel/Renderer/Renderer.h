//
// Created by stang on 25-6-12.
//

#ifndef RENDERER_H
#define RENDERER_H
#include "RendererAPI.h"


namespace Hazel
{
    class Shader;
    class OrthographicCamera;
}

namespace Hazel
{
    class HAZEL_API Renderer
    {
    public:
        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        static void BegeinScene(OrthographicCamera& camera);
        static void Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray);
        static void EndScene();
        static void SetTranform(glm::mat4 transform);

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
            glm::mat4 transform = glm::mat4(1.0f);
        };
        static SceneData* s_SceneData;
    };
}


#endif //RENDERER_H
