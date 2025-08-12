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
        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Enable depth testing with early-Z optimizations
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL); // Use LEQUAL for better early-Z performance
        glDepthMask(GL_TRUE);   // Enable depth writes
        
        // Enable depth clamping for early-Z optimization if available
        if (GLAD_GL_ARB_depth_clamp) {
            glEnable(GL_DEPTH_CLAMP);
            HZ_CORE_INFO("Enabled GL_DEPTH_CLAMP for early-Z optimization");
        }
        
        // Additional early-Z optimizations
        glEnable(GL_CULL_FACE);  // Enable face culling to reduce fragment processing
        glCullFace(GL_BACK);     // Cull back faces
        glFrontFace(GL_CCW);     // Counter-clockwise winding for front faces
        
        HZ_CORE_INFO("OpenGL Renderer initialized with early-Z optimizations");
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
        glLineWidth(width);
    }

    void OpenGLRenererAPI::BeginDepthPrepass()
    {
        // Disable color writes for depth-only rendering
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        
        // Ensure depth writes are enabled
        glDepthMask(GL_TRUE);
        
        // Set depth function for prepass (typically GL_LESS)
        glDepthFunc(GL_LESS);
        
        // Clear depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);
        
        HZ_CORE_TRACE("Started depth prepass for early-Z optimization");
    }

    void OpenGLRenererAPI::EndDepthPrepass()
    {
        // Re-enable color writes
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        // Set depth function to GL_EQUAL for color pass
        // This ensures only pixels that passed depth test are shaded
        glDepthFunc(GL_EQUAL);
        
        // Disable depth writes for color pass to preserve depth buffer
        glDepthMask(GL_FALSE);
        
        HZ_CORE_TRACE("Ended depth prepass, configured for color pass");
    }

    void OpenGLRenererAPI::SetDepthFunction(uint32_t func)
    {
        glDepthFunc(func);
    }
}
