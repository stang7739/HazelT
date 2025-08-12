//
// Created by stang on 25-7-31.
//

#ifndef ENTITY_H
#define ENTITY_H
#include"Scene.h"
#include "entt.hpp"

namespace Hazel
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;
        Entity& operator=(const Entity& other) = default;

        template <typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template <typename T>
        T& GetComponent()
        {
            HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template <typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template <typename T>
        void RemoveComponent()
        {
            HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }
        bool operator==(const Entity& other) const
        {
            return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
        }
        bool operator!=(const Entity& other)const{return !(*this == other);}
        [[nodiscard]]
        entt::entity GetEntityHandle() const { return m_EntityHandle; }
        Scene* GetScene() const { return m_Scene; }

    private:
        entt::entity m_EntityHandle{entt::null};
        Scene* m_Scene = nullptr;
    };
}


#endif //ENTITY_H
