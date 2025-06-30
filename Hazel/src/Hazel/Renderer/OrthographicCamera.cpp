//
// Created by stang on 25-6-27.
//

#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
    //glm::ortho : A function used to generate an orthogonal projection matrix
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top): m_Projectionmatrix(
        glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
    {

    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        //Generate a translational transformation matrix
        //A translation matrix (according to the m_Position) is generated, a rotation matrix about the z-axis (according to the m_Rotation) is generated,
        //and finally the two are multiplied to obtain a transformation matrix that translates first and then rotates.
        //Rotate first and then translate
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position )* glm::rotate(glm::mat4(1.0f),m_Rotation,glm::vec3(0,0,1 ));
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_Projectionmatrix * m_ViewMatrix;
    }
}
