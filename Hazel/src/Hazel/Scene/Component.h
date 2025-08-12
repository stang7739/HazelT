//
// Created by stang on 25-7-30.
//

#ifndef COMPONENT_H
#define COMPONENT_H
#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/Camera.h"
#include "ScriptableEntity.h"


namespace Hazel
{
    class ScriptableEntity;

    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;

        TagComponent(const std::string& tag)
            : Tag(tag)
        {
        }
    };

    struct TransformComponent
    {
        glm::mat4 Transform{1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const glm::mat4& transform)
            : Transform(transform)
        {
        }

        operator glm::mat4&() { return Transform; }
        operator const glm::mat4&() const { return Transform; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;

        SpriteRendererComponent(const glm::vec4& color)
            : Color(color)
        {
        }
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
    // struct NativeScriptComponent
    // {
    //     ScriptableEntity* Instance = nullptr;
    //     std::function<void()> InstantiateFunction;
    //     std::function<void()> DestoryInstanceFunction;
    //     std::function<void(ScriptableEntity*)> OnCreateFunction;
    //     std::function<void(ScriptableEntity*,Timestep)> OnUpdateFunction;
    //     std::function<void(ScriptableEntity*)> OnDestroyFunction;
    //
    //     template<typename T>
    //     void Bind()
    //     {
    //         InstantiateFunction = [&]() { Instance = new T(); };
    //         DestoryInstanceFunction = [&](){delete (T*)Instance;Instance=nullptr;};
    //
    //         OnCreateFunction = [](ScriptableEntity* instance){ ((T*)instance)->OnCreate(); };
    //         OnDestroyFunction = [](ScriptableEntity* instance){ ((T*)instance)->OnDestory(); };
    //         OnUpdateFunction = [](ScriptableEntity* instance,Timestep ts){ ((T*)instance)->OnUpdate(ts); };
    //     }
    //
    // };
    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;
        //ScriptableEntity* (*InstantiateScript)(int) = nullptr;
        ScriptableEntity*(*InstantiateScript)() = nullptr;
        void (*DestoryScript)(NativeScriptComponent* nsc) = nullptr;

        template<typename T>
        void Bind()
        {
            InstantiateScript = [](){return static_cast<ScriptableEntity*>(new T());};
            DestoryScript =  [](NativeScriptComponent* nsc){delete nsc->Instance;nsc->Instance = nullptr;};

        }

    };

}

#endif //COMPONENT_H
