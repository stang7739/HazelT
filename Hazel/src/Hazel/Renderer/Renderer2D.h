//
// Created by stang on 25-7-14.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H
#include <glm/glm.hpp>

#include "Hazel/Core/Base.h"


namespace Hazel
{
    class Texture2D;
    class OrthographicCamera;


    class HAZEL_API Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Hazel::OrthographicCamera& camera);
        static void EndScene();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture,
                             float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture,
                             float tillingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

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
    };
}


#endif //RENDERER2D_H
