//
// Created by stang on 25-6-27.
//

#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H
#include<glm/glm.hpp>

namespace Hazel
{
    class HAZEL_API OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        void SetProjection(float left, float right, float bottom, float top);
        inline const glm::vec3& GetPosition() const { return m_Position; }

        inline void SetPosition(const glm::vec3& position)
        {
            m_Position = position;
            RecalculateViewMatrix();
        }

        inline float GetRotation() const { return m_Rotation; }

        inline void SetRotation(float rotation)
        {
            m_Rotation = glm::radians(rotation);
            RecalculateViewMatrix();
        }


        inline const glm::mat4& GetProjectionMatrix() const { return m_Projectionmatrix; }
        inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();
        glm::mat4 m_Projectionmatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;
        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        float m_Rotation = 0.0f; // in radians
    };
}


#endif //ORTHOGRAPHICCAMERA_H
