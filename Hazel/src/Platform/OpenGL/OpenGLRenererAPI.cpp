//
// Created by stang on 25-6-12.
//

#include "OpenGLRenererAPI.h"

#include <glad/glad.h>


#include "GLFW/glfw3.h"
#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
    void OpenGLRenererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {

        glViewport(x,y,width,height);
        HZ_INFO("Viewport set to x: {}, y: {}, width: {}, height: {}", x, y, width, height);

    }

    void OpenGLRenererAPI::Init()
    {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        //Controls how the source and target colors are mixed

    }

    void OpenGLRenererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t indexCount)
    {
        uint32_t count = indexCount ?indexCount : vertexArray->GetIndexBuffer()->GetCount() ;
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind any texture after drawing
    }

    void OpenGLRenererAPI::DrawLine(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
    }

    void OpenGLRenererAPI::SetLineWidth(float width)
    {
    }
}
