//
// Created by stang on 25-7-30.
//
#include "hzpch.h"
#include "Scene.h"
#include "Component.h"
#include "Entity.h"
#include <glm/glm.hpp>

#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/EditorCamera.h"
#include "Hazel/Renderer/Renderer2D.h"
// Box2D
// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Hazel
{
    static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType type)
    {
        switch(type)
        {
        case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
        case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }
        HZ_CORE_ASSERT(false,"Unknown Rigidbody2DComponent::BodyType");
        return b2_staticBody;
    }
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name, glm::vec3 position)
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
                    nsc.Instance->m_Entity = Entity{entity, this};

                    nsc.Instance->OnCreate();
                }

                nsc.Instance->OnUpdate(ts);
            });
        }
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto group = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : group)
            {
                auto& transform = group.get<TransformComponent>(entity);
                auto& camera = group.get<CameraComponent>(entity);
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
        }
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, cameraTransform);
            auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
            for (auto entity : group)
            {
                auto& transform = group.get<TransformComponent>(entity);
                auto& sprite = group.get<SpriteRendererComponent>(entity);

                Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
            }
            Renderer2D::EndScene();
        }
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto entity : group)
        {
            auto& transform = group.get<TransformComponent>(entity);
            auto& sprite = group.get<SpriteRendererComponent>(entity);

            Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
        }
        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRuntime(Timestep ts)
    {
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity,auto& nsc)
            {
                if(!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = Entity{entity,this};
                    nsc.Instance->OnCreate();
                }
                nsc.Instance->OnUpdate(ts);
            });
        }
        //Physics
        {
            const int32_t velocityIterations =6;
            const int32_t positionIterations = 2;
            m_PhysicsWorld->Step(ts,velocityIterations,positionIterations);
            auto view = m_Registry.view<Rigidbody2DComponent>();
            for(auto entt : view)
            {
                Entity entity = {entt,this};
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
                b2Body* body = (b2Body*)rb2d.RuntimeBody;
                const auto& position = body->GetPosition();
                transform.Translation.x = position.x;
                transform.Translation.y = position.y;
                transform.Rotation.z = body->GetAngle();
            }
        }
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_Registry.view<TransformComponent,CameraComponent>();
            for(auto entity : view)
            {
                auto[transform,camera] = view.get<TransformComponent,CameraComponent>(entity);
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
            Renderer2D::BeginScene(*mainCamera,cameraTransform);
            auto view = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for(auto entity : view)
            {

                auto [transform,sprite] = view.get<TransformComponent,SpriteRendererComponent>(entity);
                Renderer2D::DrawSprite(transform.GetTransform(),sprite,(int)entity);
            }
            Renderer2D::EndScene();
        }


    }

    void Scene::OnRuntimeStart()
    {
        m_PhysicsWorld = new b2World({0.f,-9.8f});
        auto view = m_Registry.view<Rigidbody2DComponent>();

        for(auto entt : view)
        {
            Entity entity = {entt,this};
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();
            b2BodyDef bodyDef;
            bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
            bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
            bodyDef.angle = transform.Rotation.z;

            b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
            body->SetFixedRotation(rb2d.FixedRotation);
            rb2d.RuntimeBody = body;

            if(entity.HasComponent<BoxCollider2DComponent>())
            {
                auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
                b2PolygonShape boxShape;
                boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x ,bc2d.Size.y * transform.Scale.y);
                b2FixtureDef fixtureDef;
                fixtureDef.shape =&boxShape;
                fixtureDef.density = bc2d.Density;
                fixtureDef.friction = bc2d.Friction;
                fixtureDef.restitution = bc2d.Restitution;
                fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
                body->CreateFixture(&fixtureDef);


            }

        }
    }

    void Scene::OnRuntimeStop()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportsize(width, height);
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            const auto& cameraComponent = view.get<CameraComponent>(entity);
            if (cameraComponent.Primary)
            {
                return Entity(entity, this);
            }
        }
    }

    template <typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }

    template <>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        if (m_ViewportHeight > 0 && m_ViewportWidth > 0)
        {
            component.Camera.SetViewportsize(m_ViewportWidth, m_ViewportHeight);
        }
    }

    template <>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
    {
    }
}
