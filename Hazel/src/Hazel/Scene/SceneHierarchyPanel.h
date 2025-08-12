//
// Created by stang on 25-8-12.
//

#ifndef SCENEHIERARCHYPANEL_H
#define SCENEHIERARCHYPANEL_H
#include "Entity.h"
#include "Hazel/Core/Base.h"


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
    private:
        void DrawEntityNode(Entity entityID);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}


#endif //SCENEHIERARCHYPANEL_H
