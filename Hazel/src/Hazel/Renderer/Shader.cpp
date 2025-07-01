//
// Created by stang on 25-6-24.
//
#include <hzpch.h>
#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Hazel
{
Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    switch(Renderer::GetAPI())
    {
    case RendererAPI::API::None:
        HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    case RendererAPI::API::OpenGL:
        return new OpenGLShader(vertexSrc, fragmentSrc);
    }

}

}
