//
// Created by stang on 25-7-14.
//

#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>

#include "Buffer.h"
#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;

        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data = new Renderer2DStorage();

    void Renderer2D::Init()
    {
        s_Data->QuadVertexArray = VertexArray::Create();
        float squareVertices[5 * 4] = {

            -0.5f, -0.5f, 0.0f, 0.f, 0.f,
            0.5f, -0.5f, 0.0f, 1.f, 0.f,
            0.5f, 0.5f, 0.0f, 1.f, 1.f,
            -0.5f, 0.5f, 0.0f, 0.f, 1.f
        };
        Ref<VertexBuffer> squarevertexBuffer;
        squarevertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        // HZ_CORE_TRACE("squarevertexBuffer called, ptr = {0}", (void*)squarevertexBuffer.get());


        squarevertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });
        s_Data->QuadVertexArray->AddVertexBuffer(squarevertexBuffer);
        unsigned int squareIndices[6] = {
            0, 1, 2, // Triangle 1
            2, 3, 0 // Triangle 2
        };
        Ref<IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.
            reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
        s_Data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);
        // HZ_CORE_TRACE("squareIndexBuffer called, ptr = {0}", (void*)squareIndexBuffer.get());

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff; // White color in RGBA format

        s_Data->WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));


        s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);

        // m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
        // m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");
    }

    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_Data->TextureShader->Bind();
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
            glm::mat4(1.0f), glm::vec3(size, 1.0f));
        s_Data->TextureShader->SetMat4("u_Transform", transform);
        s_Data->TextureShader->SetFloat4("u_Color", color);
        s_Data->TextureShader->SetFloat("u_TillingFactor", 1.0f); // Set default tilling factor
        s_Data->WhiteTexture->Bind(0); // Bind the white texture

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture,
                              float tillingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture,
                              float tillingFactor, const glm::vec4& tintColor)
    {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetFloat4("u_Color", tintColor); // Set color to white
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
            glm::mat4(1.0f), glm::vec3(size, 1.0f));
        s_Data->TextureShader->SetMat4("u_Transform", transform);
        s_Data->TextureShader->SetFloat("u_TillingFactor", tillingFactor); // Set the tilling factor
        texture->Bind(0);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetFloat4("u_Color", color);
        s_Data->TextureShader->SetFloat("u_TillingFactor", 1.0f); // Set default tilling factor
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
            glm::mat4(1.f), glm::vec3(size, 1.f));
        s_Data->TextureShader->SetMat4("u_Transform", transform);
        s_Data->WhiteTexture->Bind(0); // Bind the white texture
        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D> texture,
                                     float tillingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D> texture,
                                     float tillingFactor, const glm::vec4& tintColor)
    {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetFloat4("u_Color", tintColor);
        s_Data->TextureShader->SetFloat("u_TillingFactor", tillingFactor); // Set default tilling factor
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
            glm::mat4(1.f), glm::vec3(size, 1.f));
        s_Data->TextureShader->SetMat4("u_Transform", transform);
        texture->Bind(0); // Bind the white texture
        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}
