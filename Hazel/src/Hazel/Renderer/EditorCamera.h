//
// Created by stang on 25-9-6.
//

#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H
#include <glm/glm.hpp>

#include "Camera.h"


namespace Hazel
{
    class MouseScrolledEvent;
    class Event;
    class Timestep;

    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearclip, float farclip);
        ~EditorCamera() = default;
        void SetViewportSize(float width, float height);
        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);
        bool OnMouseScroll(MouseScrolledEvent& e);

        glm::vec3 GetForwardDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetUpDirection() const;
        glm::quat GetOrientation() const;
        inline const glm::vec3& GetPosition() const { return m_Position; }
        inline float GetPitch() const { return m_Pitch; }
        inline float GetYaw() const { return m_Yaw; }
        inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        inline glm::mat4 GetViewProjectionMatrix() const { return m_Projection * m_ViewMatrix; }
        inline float GetDistance() const { return m_Distance; }
        inline float GetViewportWidth() const {return m_ViewportWidth; }
        inline float GetViewportHeight() const {return m_ViewportHeight; }
        inline float GetNearClip() const { return m_NearClip; }
        inline float GetFarClip() const { return m_FarClip; }
        inline float GetFOV() { return m_FOV; }

        inline void SetDistance(float distance)
        {
            m_Distance = distance;
            UpdateView();
        }

        inline void SetVireportSize(float width, float heigiht)
        {
            m_ViewportWidth = width;
            m_ViewportHeight = heigiht;
            UpdateProjection();
        }

    private:
        void UpdateView();
        void UpdateProjection();
        glm::vec3 CalculatePosition() const;
        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);
        std::pair<float, float> PanSpeed();
        float RotationSpeed() const;
        float ZoomSpeed() const;
        glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        float m_FOV = 45.f;
        float m_AspectRatio = 1.778f;
        float m_NearClip = 0.1f;
        float m_FarClip = 1000.f;
        glm::mat4 m_ViewMatrix;
        glm::vec3 m_FocalPoint= {0.f,0.f,0.f};
        glm::vec3 m_Position = {0.f,0.f,0.f};
        glm::vec2 m_InitialMousePosition = {0.f, 0.f};
        float m_Distance = 10.f;
        float m_Pitch = 0.f, m_Yaw = 0.f;
        float m_ViewportWidth = 1280.f, m_ViewportHeight = 720.f;
    };
}


#endif //EDITORCAMERA_H
