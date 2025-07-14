//
// Created by stang on 25-7-1.
//

#include "OpenGLShader.h"
//
// Created by stang on 25-6-24.
//
#include <fstream>
#include <hzpch.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Hazel
{
    GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
    {
        //Create an empty vertex shader handle
        if (type == "vertex")
        {
            return GL_VERTEX_SHADER;
        }
        if (type == "fragment" || type == "pixel")
        {
            return GL_FRAGMENT_SHADER;
        }
        //Send the vertex shader source code to GL
        //Note the std::string's .c_str is NULL character terminated.
        HZ_CORE_ASSERT(false, "Unknown shader type specified!");
        return 0; // Return 0 if the type is not recognized
    }

    OpenGLShader::OpenGLShader(const std::string& filePath)
    {
        std::string source = ReadFile(filePath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        //Extract name from filepath
        auto lastSlash = filePath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filePath.rfind('.');
        auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filePath.substr(lastSlash,count);
    }

    OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc):
        m_Name(name)
    {
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(sources);
    }

    std::string OpenGLShader::ReadFile(const std::string& filePath)
    {
        std::string result;
        std::ifstream in(filePath, std::ios::in| std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            HZ_CORE_ERROR("Could not open file {0}", filePath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        //Find the place where the first type name is
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error in shader file");

            size_t begin = pos + typeTokenLength + 1; // +1 for the space after the type
            std::string type = source.substr(begin, eol - begin);
            HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos,
                                                                      pos - (nextLinePos == std::string::npos
                                                                                 ? source.size() - 1
                                                                                 : nextLinePos));
        }
        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        // Create an empty vertex shader handle
        GLuint program = glCreateProgram();
        std::vector<GLenum> glShaderIDs;
        glShaderIDs.reserve(shaderSources.size());
        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;
            GLuint Shader = glCreateShader(type);
            // Send the vertex shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(Shader, 1, &sourceCStr, 0);

            // Compile the vertex shader
            glCompileShader(Shader);

            GLint isCompiled = 0;
            glGetShaderiv(Shader, GL_COMPILE_STATUS, &isCompiled);

            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(Shader, maxLength, &maxLength, &infoLog[0]);

                // The maxLength includes the NULL character

                // We don't need the shader anymore.
                glDeleteShader(Shader);

                HZ_CORE_ERROR("{0}", infoLog.data());
                HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
                break;
            }
            glAttachShader(program, Shader);
            glShaderIDs.push_back(Shader);
        }
        m_RendererID = program;

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            for (auto id : glShaderIDs)
                glDeleteShader(id);

            HZ_CORE_ERROR("{0}", infoLog.data());
            HZ_CORE_ASSERT(false, "Shader link failure!");
            return;
        }

        for (auto id : glShaderIDs)
            glDetachShader(program, id);
    }

    OpenGLShader::~OpenGLShader()
    {

        glDeleteProgram(m_RendererID);
        // m_RendererID = 0;
        // HZ_CORE_INFO("Shader destroyed");
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
        // HZ_CORE_INFO("Shader unbound");
    }

     void OpenGLShader::SetInt(const std::string& name, int value) {
        UploadUniformInt(name,value);
     }
     void OpenGLShader::SetFloat(const std::string& name, float value) {
        UploadUniformFloat(name, value);
     }
     void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
        UploadUniformFloat2(name,value);
     }
     void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
        UploadUniformFloat3(name, value);
     }
     void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
        UploadUniformFloat4(name, value);

     }
     void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
        UploadUniformMat3(name, matrix);

     }
     void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
        UploadUniformMat4(name, matrix);
     }


    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& Matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1,GL_FALSE, glm::value_ptr(Matrix));
    }


    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& ViewProjectionMatrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1,GL_FALSE, glm::value_ptr(ViewProjectionMatrix));
    }
}
