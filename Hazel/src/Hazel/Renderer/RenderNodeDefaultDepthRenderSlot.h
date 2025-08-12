//
// Created for HazelT - Default Depth Render Slot
//

#ifndef RENDERNODEDEFAULTDEPTHRENDERSLOT_H
#define RENDERNODEDEFAULTDEPTHRENDERSLOT_H

#include "RenderGraph.h"
#include "RenderCommand.h"

namespace Hazel
{
    class RenderNodeDefaultDepthRenderSlot : public RenderNode
    {
    public:
        RenderNodeDefaultDepthRenderSlot();
        virtual ~RenderNodeDefaultDepthRenderSlot() = default;

        virtual void Execute() override;
        virtual const std::string& GetName() const override;

        // Depth-specific settings
        void SetEarlyZEnabled(bool enabled) { m_EarlyZEnabled = enabled; }
        bool IsEarlyZEnabled() const { return m_EarlyZEnabled; }
        
        void SetDepthPrepassEnabled(bool enabled) { m_DepthPrepassEnabled = enabled; }
        bool IsDepthPrepassEnabled() const { return m_DepthPrepassEnabled; }

    private:
        void ExecuteDepthPrepass();
        void ConfigureDepthState();
        void RestoreDepthState();

    private:
        std::string m_Name = "DefaultDepthRenderSlot";
        bool m_EarlyZEnabled = true;
        bool m_DepthPrepassEnabled = true;
        
        // Store original depth state for restoration
        bool m_OriginalDepthTest = false;
        int m_OriginalDepthFunc = 0;
        bool m_OriginalDepthMask = false;
    };
}

#endif // RENDERNODEDEFAULTDEPTHRENDERSLOT_H