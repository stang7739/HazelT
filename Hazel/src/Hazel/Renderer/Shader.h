//
// Created by stang on 25-6-24.
//

#ifndef SHADER_H
#define SHADER_H
#include <glm/glm.hpp>


namespace Hazel
{
    class HAZEL_API Shader {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();
        void Bind() const;
        void Unbind() const;
        void UploadUniformMat4(const std::string& name,glm::mat4& ViewProjectionMatrix);

    private:
        uint32_t m_RendererID;
    };
}



#endif //SHADER_H
