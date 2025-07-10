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
        static void init();
        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        static void BegeinScene(OrthographicCamera& camera);
        static void Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray);
        static void EndScene();
        static void SetTranform(glm::vec3 squarePosition,float squareRotation);
        static void OnWindowResize(uint32_t width, uint32_t height);

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
