//
// Created by stang on 25-7-30.
//

#ifndef SCENE_H
#define SCENE_H
#include <entt.hpp>
#include <string>

namespace Hazel
{
    class Timestep;
    class Entity;

    class HAZEL_API Scene {
    public:
        Scene();
        ~Scene();
        Entity CreateEntity(const std::string& name = std::string());
        void OnUpdate(Timestep ts);
        entt::registry& Reg() { return m_Registry; }
    private:
        entt::registry m_Registry;
        friend class Entity;
    };
}



#endif //SCENE_H
