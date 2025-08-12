//
// Created for HazelT - Default Depth Render Slot Implementation
//

#include "RenderNodeDefaultDepthRenderSlot.h"
#include "Hazel/Core/Log.h"
#include <glad/glad.h>

namespace Hazel
{
    RenderNodeDefaultDepthRenderSlot::RenderNodeDefaultDepthRenderSlot()
    {
        HZ_CORE_INFO("Created DefaultDepthRenderSlot with Early-Z optimization");
    }

    void RenderNodeDefaultDepthRenderSlot::Execute()
    {
        if (!IsEnabled())
            return;

        HZ_CORE_TRACE("Executing DefaultDepthRenderSlot");

        // Configure depth state for early-Z optimization
        ConfigureDepthState();

        // Execute depth prepass if enabled
        if (m_DepthPrepassEnabled)
        {
            ExecuteDepthPrepass();
        }

        // Restore previous depth state
        RestoreDepthState();
    }

    const std::string& RenderNodeDefaultDepthRenderSlot::GetName() const
    {
        return m_Name;
    }

    void RenderNodeDefaultDepthRenderSlot::ConfigureDepthState()
    {
        // Store original state
        m_OriginalDepthTest = glIsEnabled(GL_DEPTH_TEST);
        glGetIntegerv(GL_DEPTH_FUNC, &m_OriginalDepthFunc);
        GLboolean depthMask;
        glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
        m_OriginalDepthMask = depthMask == GL_TRUE;

        // Configure for early-Z optimization
        glEnable(GL_DEPTH_TEST);
        
        if (m_EarlyZEnabled)
        {
            // Use GL_LEQUAL for better early-Z rejection
            glDepthFunc(GL_LEQUAL);
            
            // Enable depth writes for prepass
            glDepthMask(GL_TRUE);
            
            // Enable depth clamping if available (helps with early-Z)
            if (GLAD_GL_ARB_depth_clamp)
            {
                glEnable(GL_DEPTH_CLAMP);
            }
        }
        else
        {
            // Standard depth testing
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
        }

        HZ_CORE_TRACE("Configured depth state - Early-Z: {0}, Depth Prepass: {1}", 
                     m_EarlyZEnabled, m_DepthPrepassEnabled);
    }

    void RenderNodeDefaultDepthRenderSlot::ExecuteDepthPrepass()
    {
        HZ_CORE_TRACE("Executing depth prepass for early-Z optimization");
        
        // Use the render command system for depth prepass
        RenderCommand::BeginDepthPrepass();
        
        // TODO: Render depth-only passes for opaque geometry here
        // This would typically involve:
        // 1. Binding a depth-only shader
        // 2. Rendering all opaque objects front-to-back
        // 3. Using simplified vertex shaders without fragment processing
        
        // End the depth prepass and configure for color rendering
        RenderCommand::EndDepthPrepass();
    }

    void RenderNodeDefaultDepthRenderSlot::RestoreDepthState()
    {
        // Restore original OpenGL depth state
        if (m_OriginalDepthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
            
        glDepthFunc(m_OriginalDepthFunc);
        glDepthMask(m_OriginalDepthMask ? GL_TRUE : GL_FALSE);
        
        // Disable depth clamping if we enabled it
        if (m_EarlyZEnabled && GLAD_GL_ARB_depth_clamp)
        {
            glDisable(GL_DEPTH_CLAMP);
        }
        
        HZ_CORE_TRACE("Restored original depth state");
    }
}