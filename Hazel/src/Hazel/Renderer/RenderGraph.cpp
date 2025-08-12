//
// Created for HazelT - Render Graph System Implementation
//

#include "RenderGraph.h"
#include "Hazel/Core/Log.h"
#include <fstream>
#include <algorithm>

namespace Hazel
{
    void RenderGraph::AddNode(Ref<RenderNode> node)
    {
        if (node)
        {
            m_RenderNodes.push_back(node);
            HZ_CORE_INFO("Added render node: {0}", node->GetName());
        }
    }

    void RenderGraph::RemoveNode(const std::string& name)
    {
        auto it = std::remove_if(m_RenderNodes.begin(), m_RenderNodes.end(),
            [&name](const Ref<RenderNode>& node) {
                return node->GetName() == name;
            });
        
        if (it != m_RenderNodes.end())
        {
            m_RenderNodes.erase(it, m_RenderNodes.end());
            HZ_CORE_INFO("Removed render node: {0}", name);
        }
    }

    void RenderGraph::ExecuteGraph()
    {
        for (auto& node : m_RenderNodes)
        {
            if (node && node->IsEnabled())
            {
                node->Execute();
            }
        }
    }

    Ref<RenderNode> RenderGraph::GetNode(const std::string& name)
    {
        auto it = std::find_if(m_RenderNodes.begin(), m_RenderNodes.end(),
            [&name](const Ref<RenderNode>& node) {
                return node->GetName() == name;
            });
        
        return (it != m_RenderNodes.end()) ? *it : nullptr;
    }

    void RenderGraph::LoadFromFile(const std::string& filepath)
    {
        // For now, this is a placeholder
        // In a full implementation, this would parse the lwrp.rng file
        HZ_CORE_INFO("Loading render graph from: {0}", filepath);
    }
}