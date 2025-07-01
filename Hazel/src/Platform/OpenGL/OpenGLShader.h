//
// Created by stang on 25-7-1.
//

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H
#include <glm/fwd.hpp>

#include "Hazel/Renderer/Shader.h"


namespace Hazel
{
    class OpenGLShader : public Shader{
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();
        virtual void Bind() const override;
        virtual void Unbind() const override;
         void UploadUniformFloat(const std::string& name, float value);
         void UploadUniformFloat2(const std::string& name,const glm::vec2& value);
         void UploadUniformFloat3(const std::string& name,const glm::vec3& value);
         void UploadUniformFloat4(const std::string& name,const glm::vec4& value);
         void UploadUniformMat3(const std::string& name,const glm::mat3& Matrix);
         void UploadUniformMat4(const std::string& name, const glm::mat4& ViewProjectionMatrix);

    private:
        uint32_t m_RendererID;
    };
}



#endif //OPENGLSHADER_H
