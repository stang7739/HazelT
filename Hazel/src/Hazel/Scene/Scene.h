//
// Created by stang on 25-7-30.
//

#ifndef SCENE_H
#define SCENE_H

#include <entt.hpp>
#include <string>
#include <glm/glm.hpp>

#include "Hazel/Core/Base.h"
#include "Hazel/Core/UUID.h"

class b2World;

namespace Hazel
{
    class EditorCamera;
    class Timestep;
    class Entity;

    class  Scene {
    public:
        Scene();
        ~Scene();
        static Ref<Scene>Copy(Ref<Scene> other);
        Entity CreateEntity(const std::string& name = std::string(),glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
        Entity CreateEntityWithUUID(UUID uuid,const std::string& name = std::string(),glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
        void DestoryEntity(Entity entity);
        // void OnUpdate(Timestep ts);
        void OnUpdateRuntime(Timestep ts);
        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnUpdateEditor(Timestep ts,EditorCamera& camera);
        entt::registry& Reg() { return m_Registry; }
        void OnViewportResize(uint32_t width,uint32_t height);
        Entity GetPrimaryCameraEntity();
        void DuplicateEntity(Entity entity);

        template<typename... Components>
        auto GetAllEntitiesWith()
        {
            return m_Registry.view<Components...>();
        }


    private:
        template<typename T>
        void OnComponentAdded(Entity entity,T& component);
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0,m_ViewportHeight;
        b2World* m_PhysicsWorld = nullptr;
        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}



#endif //SCENE_H
