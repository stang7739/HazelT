//
// Created by stang on 25-7-1.
//

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H
#include <glm/fwd.hpp>

#include "Hazel/Renderer/Shader.h"

// TODO: REMOVE!
typedef unsigned int GLenum;
namespace Hazel
{
    class HAZEL_API OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filePath);
        OpenGLShader(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const std::string& GetName() const override { return m_Name; }

        virtual void SetInt(const std::string& name, int value) override;
        virtual void SetFloat(const std::string& name, float value) override;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
        virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
        void UploadUniformMat3(const std::string& name, const glm::mat3& Matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& ViewProjectionMatrix);

    private:
        static GLenum ShaderTypeFromString(const std::string& type);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
        std::string ReadFile(const std::string& filePath);
        std::unordered_map<GLenum,std::string> PreProcess(const std::string source);
        uint32_t m_RendererID;
        std::string m_Name;
    };
}


#endif //OPENGLSHADER_H
