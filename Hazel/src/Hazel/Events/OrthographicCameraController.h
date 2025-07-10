//
// Created by stang on 25-7-9.
//

#ifndef ORTHOGRAPHICCAMERACONTROLLER_H
#define ORTHOGRAPHICCAMERACONTROLLER_H
#include "Hazel/Renderer/OrthographicCamera.h"


namespace Hazel
{
    class MouseScrolledEvent;
}

namespace Hazel
{
    class WindowResizeEvent;
    class Event;
    class Timestep;
}

namespace Hazel
{
    class HAZEL_API OrthographicCameraController {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);
        void OnUpdate(Timestep timestep);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        bool m_Rotation;
        OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

        float m_CameraRotation = 0.0f; // in degrees, clockwise
        float c_CameraRotationSpeed = 90.f;
        float m_CameraTranslationSpeed = 9.f;
    };
}



#endif //ORTHOGRAPHICCAMERACONTROLLER_H
