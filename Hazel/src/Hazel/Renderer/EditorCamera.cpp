//
// Created by stang on 25-9-6.
//

#include "hzpch.h"
#include "EditorCamera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Hazel/Core/Input.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearclip, float farclip)
    {
        UpdateView();
    }

    void EditorCamera::SetViewportSize(float width, float height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        UpdateProjection();
    }

    void EditorCamera::OnUpdate(Timestep ts)
    {
        if(Input::IsKeyPressed(HazelKey::LeftAlt))
        {
            const glm::vec2& mouse{Input::GetMouseX(), Input::GetMouseY()};
            auto delta = (mouse - m_InitialMousePosition) * 0.003f;
            m_InitialMousePosition = mouse;
            if(Input::IsMouseButtonPressed(HazelKey::MouseMiddle))
                MousePan(delta);
            else if(Input::IsMouseButtonPressed(HazelKey::MouseLeft))
                MouseRotate(delta);
            else if(Input::IsMouseButtonPressed(HazelKey::MouseRight))
                MouseZoom(delta.y);
        }
        UpdateView();
    }
    void EditorCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }
    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
    {
        float delta = e.GetOffsetY() * 0.1f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void EditorCamera::UpdateView()
    {
         m_Position = CalculatePosition();
        auto orientation = GetOrientation();
        m_ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0), m_Position) * glm::toMat4(orientation));
    }

    void EditorCamera::UpdateProjection()
    {
        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
         m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        //绕z轴旋转的角度
        return glm::rotate(GetOrientation(), glm::vec3(0.f, 0.f, -1.f));
    }
    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
    }
    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.f, 1.f, 0.f));
    }
    glm::quat EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.f));
    }
    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        auto[xSpeed, ySpeed] = PanSpeed();
        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
    }
    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * RotationSpeed();
        m_Pitch += delta.y * RotationSpeed();
    }
    void EditorCamera::MouseZoom(float delta)
    {
        m_Distance -= delta * ZoomSpeed();
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }
    std::pair<float,float> EditorCamera::PanSpeed()
    {
        float x = std::min(m_ViewportWidth / 1000.f, 2.4f); //max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.f, 2.4f); //max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
        return {xFactor, yFactor};
    }
    float EditorCamera::RotationSpeed() const
    {
        return 0.8f;
    }
    float EditorCamera::ZoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f); // max speed = 100
        return speed;
    }
}
