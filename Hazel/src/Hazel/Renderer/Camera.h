//
// Created by stang on 25-8-1.
//

#ifndef CAMERA_H
#define CAMERA_H
#include<glm/glm.hpp>
namespace Hazel
{
    class  Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection):m_Projection(projection){}
        virtual ~Camera() = default;
        const glm::mat4& GetProjection() const {return m_Projection; }
    protected:
        glm::mat4 m_Projection = {1.0f};
    };
}
#endif //CAMERA_H
