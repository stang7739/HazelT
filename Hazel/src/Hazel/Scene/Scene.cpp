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
    static void DoMath(const glm::mat4& transform)
    {

    }

    static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
    {

    }

    Scene::Scene()
    {
#if 0 // ENTT_EXAMPLE_CODE - 临时禁用示例代码
        entt::entity entity = m_Registry.create();
        m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

        m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


        if (m_Registry.has<TransformComponent>(entity))
            TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


        auto view = m_Registry.view<TransformComponent>();
        for (auto entity : view)
        {
            TransformComponent& transform = view.get<TransformComponent>(entity);
        }

        auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : group)
        {
            auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
        }
#endif
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = {m_Registry.create(), this};
        
        // 关键测试：分别测试不同的调用
        HZ_CORE_INFO("=== Critical Test ===");
        HZ_CORE_INFO("Scene object address: {}", (void*)this);
        HZ_CORE_INFO("Registry address: {}", (void*)&m_Registry);
        HZ_CORE_INFO("Entity created with ID: {}", (uint32_t)entity.GetEntityHandle());
        
        // 测试1：直接使用 registry 检查
        bool directCheck = m_Registry.has<SpriteRendererComponent>(entity.GetEntityHandle());
        HZ_CORE_INFO("Direct registry check: {}", directCheck);
        
        // 测试2：通过 Entity wrapper 检查（这可能是问题的触发点）
        bool wrapperCheck = entity.HasComponent<SpriteRendererComponent>();
        HZ_CORE_INFO("Wrapper check: {}", wrapperCheck);
        
        // 测试3：再次检查，看看是否有变化
        bool secondCheck = entity.HasComponent<SpriteRendererComponent>();
        HZ_CORE_INFO("Second wrapper check: {}", secondCheck);
        
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>();
        auto& tag = entity.GetComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        
        // 验证添加的组件
        bool hasTransform = entity.HasComponent<TransformComponent>();
        bool hasTag = entity.HasComponent<TagComponent>();
        HZ_CORE_INFO("After adding components - Transform: {}, Tag: {}", hasTransform, hasTag);
        
        // 最后检查：返回前再次验证
        HZ_CORE_INFO("FINAL CHECK before return:");
        HZ_CORE_INFO("  Transform: {}", entity.HasComponent<TransformComponent>());
        HZ_CORE_INFO("  Tag: {}", entity.HasComponent<TagComponent>());
        HZ_CORE_INFO("  Sprite: {}", entity.HasComponent<SpriteRendererComponent>());
        HZ_CORE_INFO("  Entity ID: {}", (uint32_t)entity.GetEntityHandle());
        HZ_CORE_INFO("  Scene ptr: {}", (void*)entity.GetScene());

        HZ_CORE_INFO("=== End Critical Test ===");
        return entity;
    }

    void Scene::OnUpdate(Timestep ts)
    {
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        {
            auto group = m_Registry.view<TransformComponent,CameraComponent>();
            for(auto entity : group)
            {
                auto& transform = group.get<TransformComponent>(entity);
                auto& camera = group.get<CameraComponent>(entity);
                if(camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform.Transform;
                    break;
                }
            }
        }
        if(mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
            auto group = m_Registry.view<TransformComponent,SpriteRendererComponent>();
            for (auto entity : group)
            {
                auto& transform = group.get<TransformComponent>(entity);
                auto& sprite = group.get<SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.Transform, sprite.Color);
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

}