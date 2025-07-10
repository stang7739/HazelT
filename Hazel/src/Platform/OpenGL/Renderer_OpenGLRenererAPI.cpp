//
// Created by stang on 25-6-12.
//

#include "Renderer_OpenGLRenererAPI.h"

#include <glad/glad.h>


#include "GLFW/glfw3.h"
#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
    void OpenGLRenererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {

        glViewport(x,y,width,height);
        HZ_INFO("""Viewport set to x: {0}, y: {1}, width: {2}, height: {3}", x, y, width, height);

    }

    void OpenGLRenererAPI::Init()
    {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    void OpenGLRenererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRenererAPI::DrawLine(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
    }

    void OpenGLRenererAPI::SetLineWidth(float width)
    {
    }
}
