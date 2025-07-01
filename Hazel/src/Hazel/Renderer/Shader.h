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

        ~Shader() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}



#endif //SHADER_H
