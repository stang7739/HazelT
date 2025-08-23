//
// Created by stang on 25-8-12.
//

#ifndef SCENEHIERARCHYPANEL_H
#define SCENEHIERARCHYPANEL_H
#include "Entity.h"
#include "Hazel/Core/Base.h"
#include "Hazel/Core/Timestep.h"


namespace Hazel
{
    class Scene;

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);
        void SetContext(const Ref<Scene>& scene);
        void OnImGuiRender();
        void OnImGuiRender(Timestep ts);
    private:
        void DrawEntityNode(Entity entityID);
        void DrawComponents(Entity entity);

    private:
        float m_Times = 0.0f;
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}


#endif //SCENEHIERARCHYPANEL_H
