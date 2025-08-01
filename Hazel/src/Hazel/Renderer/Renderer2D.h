//
// Created by stang on 25-7-14.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H
#include <glm/glm.hpp>

#include "Hazel/Core/Base.h"


namespace Hazel
{
    class SubTexture2D;
    class Texture2D;
    class OrthographicCamera;


    class HAZEL_API Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Hazel::OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture,
                             float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture,
                             float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture,
                             float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture,
                             float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform,  const Ref<Texture2D> texture,float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));



        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D> texture, float tillingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D> texture, float tillingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const Ref<SubTexture2D> subtexture, float tillingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const Ref<SubTexture2D> subtexture, float tillingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() { return QuadCount * 6; }
        };

        static void ResetStats();
        static Statistics GetStats();

    private:
        static void FlushAndReset();
    };
}


#endif //RENDERER2D_H
