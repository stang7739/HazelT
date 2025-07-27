//
// Created by stang on 25-7-14.
//

#include "Renderer2D.h"

#include <glm/ext/matrix_transform.hpp>

#include "Buffer.h"
#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "SubTexture2D.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; //TODO :RenderCaps
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

        glm::vec4 QuadVertexPositions[4];
        Renderer2D::Statistics Stats;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create();
        // float squareVertices[5 * 4] = {
        //
        //     -0.5f, -0.5f, 0.0f, 0.f, 0.f,
        //     0.5f, -0.5f, 0.0f, 1.f, 0.f,
        //     0.5f, 0.5f, 0.0f, 1.f, 1.f,
        //     -0.5f, 0.5f, 0.0f, 0.f, 1.f
        // };
        // Ref<VertexBuffer> squarevertexBuffer;
        // squarevertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        // // HZ_CORE_TRACE("squarevertexBuffer called, ptr = {0}", (void*)squarevertexBuffer.get());

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float, "a_TexIndex"},
            {ShaderDataType::Float, "a_TilingFactor"}


        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset += 4;
        }


        // unsigned int squareIndices[6] = {
        //     0, 1, 2, // Triangle 1
        //     2, 3, 0 // Triangle 2
        // };
        // Ref<IndexBuffer> squareIndexBuffer =(IndexBuffer::Create(quad, sizeof(squareIndices) / sizeof(unsigned int)));
        Ref<IndexBuffer> squareIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(squareIndexBuffer);
        delete[] quadIndices;
        // HZ_CORE_TRACE("squareIndexBuffer called, ptr = {0}", (void*)squareIndexBuffer.get());

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff; // White color in RGBA format

        s_Data.WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }


        s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
        s_Data.TextureSlots[0] = s_Data.WhiteTexture; // Bind white texture to slot 0

        s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.f}; // Bottom left
        s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.f}; // Bottom right
        s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.f}; // Top right
        s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.f}; // Top left


        // m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
        // m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");
    }

    void Renderer2D::Shutdown()
    {
        // delete s_Data;
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1; // Reset texture slot index, 0 is reserved for white texture
    }

    void Renderer2D::EndScene()
    {
        HZ_PROFILE_FUNCTION();
        uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
        Flush();
    }

    void Renderer2D::Flush()
    {
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }

    void Renderer2D::FlushAndReset()
    {
        EndScene();
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1; // Reset texture slot index, 0 is reserved for white texture
    }


    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        // s_Data.TextureShader->Bind();
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
            glm::mat4(1.0f), glm::vec3(size, 1.0f));
        // s_Data.TextureShader->SetMat4("u_Transform", transform);
        // s_Data.TextureShader->SetFloat4("u_Color", color);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f); // Set default tilling factor
        // s_Data.WhiteTexture->Bind(0); // Bind the white texture
        //
        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
        const float textureIndex = 0.0f; // White texture index
        const float TilingFactor = 1.0f; // Default tilling factor
        //Compilation time constants
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };


        for(size_t i =0;i<quadVertexCount;i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord =textureCoords[i] ; // Default texture coordinates
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor; // Default tilling factor
            s_Data.QuadVertexBufferPtr++;
        }



        s_Data.QuadIndexCount += 6; // Each quad has 6 indices (2 triangles)
        s_Data.Stats.QuadCount++;

        // s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f); // Set default tilling factor
        //
        // s_Data.WhiteTexture->Bind(0); // Bind the white texture
        //
        // glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
        // glm::mat4(1.0f), glm::vec3(size, 1.0f));
        // s_Data.TextureShader->SetMat4("u_Transform", transform);
        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture,
                              float TilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, TilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture,
                              float TilingFactor, const glm::vec4& tintColor)
    {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
            glm::mat4(1.0f), glm::vec3(size, 1.0f));
        // constexpr glm::vec4 color = {1.f, 1.f, 1.f, 1.f}; // Default color for texture quads
        glm::vec4 color = tintColor; // Default color for texture quads
        float textureIndex = 0.f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (s_Data.TextureSlots[i].get() == texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        for(size_t i =0;i<quadVertexCount;i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i]; // Default texture coordinates
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor; // Default tilling factor
            s_Data.QuadVertexBufferPtr++;
        }



        s_Data.QuadIndexCount += 6; // Each quad has 6 indices (2 triangles)
        s_Data.Stats.QuadCount++;
        // s_Data.TextureShader->Bind();
        // s_Data.TextureShader->SetFloat4("u_Color", tintColor); // Set color to white
        // glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
        //     glm::mat4(1.0f), glm::vec3(size, 1.0f));
        // s_Data.TextureShader->SetMat4("u_Transform", transform);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", TilingFactor); // Set the tilling factor
        // texture->Bind(0);
        //
        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }
void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture,
                              float TilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, subtexture, TilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subtexture,
                              float TilingFactor, const glm::vec4& tintColor)
    {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
            glm::mat4(1.0f), glm::vec3(size, 1.0f));
        // constexpr glm::vec4 color = {1.f, 1.f, 1.f, 1.f}; // Default color for texture quads
        glm::vec4 color = tintColor; // Default color for texture quads
        float textureIndex = 0.f;
        constexpr size_t quadVertexCount = 4;

        const glm::vec2* textureCoords = subtexture->GetTexCoords();
        const Ref<Texture2D> texture = subtexture->GetTexture();

        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (s_Data.TextureSlots[i].get() == texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        for(size_t i =0;i<quadVertexCount;i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i]; // Default texture coordinates
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor; // Default tilling factor
            s_Data.QuadVertexBufferPtr++;
        }



        s_Data.QuadIndexCount += 6; // Each quad has 6 indices (2 triangles)
        s_Data.Stats.QuadCount++;
        // s_Data.TextureShader->Bind();
        // s_Data.TextureShader->SetFloat4("u_Color", tintColor); // Set color to white
        // glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
        //     glm::mat4(1.0f), glm::vec3(size, 1.0f));
        // s_Data.TextureShader->SetMat4("u_Transform", transform);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", TilingFactor); // Set the tilling factor
        // texture->Bind(0);
        //
        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
            glm::mat4(1.f), glm::vec3(size, 1.f));
        const float textureIndex = 0.0f; // White texture index
        const float TilingFactor = 1.0f; // Default tilling factor


        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        for(size_t i =0;i<quadVertexCount;i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i]; // Default texture coordinates
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor; // Default tilling factor
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6; // Each quad has 6 indices (2 triangles)
        s_Data.Stats.QuadCount++;

        // s_Data.TextureShader->Bind();
        // s_Data.TextureShader->SetFloat4("u_Color", color);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f); // Set default tilling factor
        // glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
        //     glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
        //     glm::mat4(1.f), glm::vec3(size, 1.f));
        // s_Data.TextureShader->SetMat4("u_Transform", transform);
        // s_Data.WhiteTexture->Bind(0); // Bind the white texture
        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D> texture,
                                     float TilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, TilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D> texture,
                                     float TilingFactor, const glm::vec4& tintColor)
    {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
            glm::mat4(1.f), glm::vec3(size, 1.f));
        // constexpr glm::vec4 color = {1.f, 1.f, 1.f, 1.f}; // Default color for texture quads
        glm::vec4 color = tintColor; // Default color for texture quads
        float textureIndex = 0.f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (s_Data.TextureSlots[i].get() == texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        for(size_t i =0;i<quadVertexCount;i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i]; // Default texture coordinates
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor; // Default tilling factor
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6; // Each quad has 6 indices (2 triangles)
        s_Data.Stats.QuadCount++;
        // s_Data.TextureShader->Bind();
        // s_Data.TextureShader->SetFloat4("u_Color", tintColor);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", TilingFactor); // Set default tilling factor
        // glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
        //     glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
        //     glm::mat4(1.f), glm::vec3(size, 1.f));
        // s_Data.TextureShader->SetMat4("u_Transform", transform);
        // texture->Bind(0); // Bind the white texture
        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const Ref<SubTexture2D> subtexture,
                                     float TilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, subtexture, TilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const Ref<SubTexture2D> subtexture,
                                     float TilingFactor, const glm::vec4& tintColor)
    {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();
        glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
            glm::mat4(1.f), glm::vec3(size, 1.f));
        // constexpr glm::vec4 color = {1.f, 1.f, 1.f, 1.f}; // Default color for texture quads
        glm::vec4 color = tintColor; // Default color for texture quads
        float textureIndex = 0.f;
           constexpr size_t quadVertexCount = 4;
        const glm::vec2* textureCoords = subtexture->GetTexCoords();
        const Ref<Texture2D> texture = subtexture->GetTexture();

        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (s_Data.TextureSlots[i].get() == texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        if (textureIndex == 0.f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }



        for(size_t i =0;i<quadVertexCount;i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i]; // Default texture coordinates
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor; // Default tilling factor
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6; // Each quad has 6 indices (2 triangles)
        s_Data.Stats.QuadCount++;
        // s_Data.TextureShader->Bind();
        // s_Data.TextureShader->SetFloat4("u_Color", tintColor);
        // s_Data.TextureShader->SetFloat("u_TilingFactor", TilingFactor); // Set default tilling factor
        // glm::mat transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
        //     glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * glm::scale(
        //     glm::mat4(1.f), glm::vec3(size, 1.f));
        // s_Data.TextureShader->SetMat4("u_Transform", transform);
        // texture->Bind(0); // Bind the white texture
        // s_Data.QuadVertexArray->Bind();
        // RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats,0,sizeof(Statistics));

    }

}
