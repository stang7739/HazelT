//
// Created by stang on 25-6-12.
//

#include "Renderer.h"

#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace Hazel
{
    Renderer::SceneData* Renderer::s_SceneData = new SceneData;
    void Renderer::BegeinScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::EndScene()
    {
    }
};
