//
// Created by stang on 25-8-12.
//

#ifndef SCENEHIERARCHYPANEL_H
#define SCENEHIERARCHYPANEL_H

#include "Hazel/Core/Base.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Scene/Entity.h"


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
        Entity GetSelectedEntity() const { return m_SelectionContext; }
          void SetSeletedEntity(Entity entity);
    private:
        void DrawEntityNode(Entity entityID);
        void DrawComponents(Entity entity);
        template<typename T>
        void DisplayAddComponent(const std::string& entryName);

    private:
        float m_Times = 0.0f;
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}


#endif //SCENEHIERARCHYPANEL_H
