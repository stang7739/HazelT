//
// Created by stang on 25-9-3.
//

#ifndef MATH_H
#define MATH_H
#include <glm/glm.hpp>

namespace Hazel::Math
{
        bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);;
}



#endif //MATH_H
