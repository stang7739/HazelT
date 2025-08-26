//
// Created by stang on 25-7-30.
//
#include "hzpch.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel
{


    Scene::Scene()
    {

    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name,glm::vec3 position)
    {
        Entity entity = {m_Registry.create(), this};

        entity.AddComponent<TransformComponent>(position);
        entity.AddComponent<TagComponent>();
        auto& tag = entity.GetComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }
    void Scene::DestoryEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdate(Timestep ts)
    {
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                if (!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = Entity{ entity, this };

                   nsc.Instance->OnCreate();
                }

               nsc.Instance->OnUpdate(ts);


            });
        }
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform ;
        {
            auto group = m_Registry.view<TransformComponent,CameraComponent>();
            for(auto entity : group)
            {
                auto& transform = group.get<TransformComponent>(entity);
                auto& camera = group.get<CameraComponent>(entity);
                if(camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }
        if(mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);
            auto group = m_Registry.view<TransformComponent,SpriteRendererComponent>();
            for (auto entity : group)
            {
                auto& transform = group.get<TransformComponent>(entity);
                auto& sprite = group.get<SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }
            Renderer2D::EndScene();
        }


    }
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        auto view = m_Registry.view<CameraComponent>();
        for(auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if(!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportsize(width,height);
            }
        }
    }
    template <typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }
    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }
    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }
    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity,CameraComponent& component)
    {
        component.Camera.SetViewportsize(m_ViewportWidth,m_ViewportHeight);
    }
    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
    }
    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {
    }


}