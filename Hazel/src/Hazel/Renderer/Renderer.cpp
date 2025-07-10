//
// Created by stang on 25-6-12.
//

#include "Renderer.h"

#include <glm/ext/matrix_transform.hpp>

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
    void Renderer::init()
    {
        RenderCommand::init();
    }


    Renderer::SceneData* Renderer::s_SceneData = new SceneData;
    void Renderer::BegeinScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray)
    {
        // shader->Bind();
        // shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        // shader->UploadUniformMat4("u_tranform", s_SceneData->transform);
        //The polymorphic design of abstract base classes requires runtime type recognition to safely invoke derived class-specific methods.
        //The rationale for this approach depends on the specific needs
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_tranform", s_SceneData->transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::EndScene()
    {
    }
     void Renderer::SetTranform(glm::vec3 squarePosition,float squareRotation)
    {
        auto c_Tranform = glm::translate(glm::mat4(1.0f), squarePosition)
           * glm::rotate(glm::mat4(1.0f), glm::radians(squareRotation), glm::vec3(0, 0, 1))
           * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
        s_SceneData->transform = c_Tranform;
    }
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0,0,width,height);
    }

};
