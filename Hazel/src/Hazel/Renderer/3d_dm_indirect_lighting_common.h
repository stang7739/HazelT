//
// Created for 3D DM indirect lighting common functionality
//

#ifndef DM_INDIRECT_LIGHTING_COMMON_H
#define DM_INDIRECT_LIGHTING_COMMON_H

#include "Hazel/Core/Core.h"
#include <glm/glm.hpp>

namespace Hazel
{
    /**
     * Unpack IBL irradiance using Spherical Harmonics
     * Uses CORE_DM_CONSTANT_ID_SH_ORDER to determine the SH order
     */
    inline glm::vec3 unpackIblIrradianceSH(const glm::vec3& normal, const float* shCoefficients)
    {
        // Use the constant for SH order calculation
        constexpr int shOrder = CORE_DM_CONSTANT_ID_SH_ORDER;
        constexpr int numCoefficients = shOrder * shOrder; // For SH order 3, this gives 9 coefficients
        
        glm::vec3 irradiance(0.0f);
        
        // Basic SH evaluation (simplified example)
        // In a real implementation, this would evaluate the SH basis functions
        // For SH order 3 (9 coefficients), we'd evaluate bands 0, 1, and 2
        
        // Band 0 (1 coefficient)
        if (numCoefficients >= 1)
        {
            irradiance += glm::vec3(shCoefficients[0], shCoefficients[1], shCoefficients[2]) * 0.282095f;
        }
        
        // Band 1 (3 coefficients)
        if (numCoefficients >= 4)
        {
            irradiance += glm::vec3(shCoefficients[3], shCoefficients[4], shCoefficients[5]) * 0.488603f * normal.y;
            irradiance += glm::vec3(shCoefficients[6], shCoefficients[7], shCoefficients[8]) * 0.488603f * normal.z;
        }
        
        return irradiance;
    }
}

#endif // DM_INDIRECT_LIGHTING_COMMON_H
