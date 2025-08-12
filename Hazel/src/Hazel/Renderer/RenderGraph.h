//
// Created for HazelT - Render Graph System
//

#ifndef RENDERGRAPH_H
#define RENDERGRAPH_H

#include "Hazel/Core/Base.h"
#include <vector>
#include <memory>
#include <string>

namespace Hazel
{
    class RenderNode
    {
    public:
        virtual ~RenderNode() = default;
        virtual void Execute() = 0;
        virtual const std::string& GetName() const = 0;
        virtual void SetEnabled(bool enabled) { m_Enabled = enabled; }
        virtual bool IsEnabled() const { return m_Enabled; }

    protected:
        bool m_Enabled = true;
    };

    class RenderGraph
    {
    public:
        RenderGraph() = default;
        ~RenderGraph() = default;

        void AddNode(Ref<RenderNode> node);
        void RemoveNode(const std::string& name);
        void ExecuteGraph();
        void LoadFromFile(const std::string& filepath);
        
        Ref<RenderNode> GetNode(const std::string& name);
        
    private:
        std::vector<Ref<RenderNode>> m_RenderNodes;
    };
}

#endif // RENDERGRAPH_H