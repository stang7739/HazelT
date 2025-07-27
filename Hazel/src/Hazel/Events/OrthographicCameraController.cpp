//
// Created by stang on 25-7-9.
//

#include "OrthographicCameraController.h"

#include "ApplicationEvent.h"
#include "Event.h"
#include "MouseEvent.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation )
    :m_AspectRatio(aspectRatio),m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),m_Rotation(rotation)
    {
        // m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

    }

    void OrthographicCameraController::OnUpdate(Timestep timestep)
    {
        if (Hazel::Input::IsKeyPressed(HazelKey::LeftArrow))
        {
            m_CameraPosition.x -= m_CameraTranslationSpeed * timestep; // Move left
            HZ_INFO("LEFT ARROW PRESSED");
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::RightArrow))
        {
            m_CameraPosition.x += m_CameraTranslationSpeed * timestep; // Move right
        }
        if (Hazel::Input::IsKeyPressed(HazelKey::UpArrow))
        {
            m_CameraPosition.y += m_CameraTranslationSpeed * timestep; // Move up
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::DownArrow))
        {
            m_CameraPosition.y -= m_CameraTranslationSpeed * timestep; // Move down
        }

        if(m_Rotation){
            if (Hazel::Input::IsKeyPressed(HazelKey::A))
            {
                m_CameraRotation += c_CameraRotationSpeed * timestep; // Move up
            }
            else if (Hazel::Input::IsKeyPressed(HazelKey::D))
            {
                m_CameraRotation -= c_CameraRotationSpeed * timestep; // Move down
            }

            m_Camera.SetRotation(m_CameraRotation);

        }
        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }
    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

    }

    void OrthographicCameraController::CalculateView()
    {

    }


    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= static_cast<MouseScrolledEvent&>(e).GetOffsetY() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f); // Prevent zooming out too far
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false; // Return false to allow further processing of the event
    }
    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        if(e.GetHeight() ==0 || e.GetWidth() == 0)
        {

            return false; // Return false to allow further processing of the event
        }
        m_AspectRatio = static_cast<WindowResizeEvent&>(e).GetWidth() /  static_cast<WindowResizeEvent&>(e).GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false; // Return false to allow further processing of the event
    }
}
