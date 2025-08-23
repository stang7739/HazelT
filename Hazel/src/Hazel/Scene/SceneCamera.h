//
// Created by stang on 25-8-4.
//

#ifndef SCENECAMERA_H
#define SCENECAMERA_H
#include "Hazel/Renderer/Camera.h"


namespace Hazel{
class HAZEL_API SceneCamera : public Camera {
public:
    enum class ProjectionType
    {
        Perspective = 0,
        Orthographic = 1
    };
    SceneCamera();
    virtual ~SceneCamera() = default;
    void SetOrthographic(float size,float nearClip,float farclip);
    void SetPerspective(float verticalFOV,float nearClip,float farClip);
    void SetViewportsize(uint32_t width,uint32_t height);
    float GetPerspectiveVerticalFOV() const {return m_PerspectiveFOV;}
    void SetPerspectiveVerticalFOV(float verticalFov){m_PerspectiveFOV = verticalFov;RecalculateProjection();}
    float GetPerspectiveNearClip() const {return m_PerspectiveNear;}
    void SetPerspectiveNearClip(float nearClip){m_PerspectiveNear = nearClip;RecalculateProjection();}
    float GetPerspectiveFarClip() const {return m_PerspectiveFar;}
    void SetPerspectiveFarClip(float farClip){m_PerspectiveFar = farClip;RecalculateProjection();}

    float GetOrthographicSize() const {return m_OrthographicSize;}
    void SetOrthographicSize(float size){m_OrthographicSize = size;RecalculateProjection();}
    float GetOrthographicNearClip() const {return m_OrthographicNear;}
    void SetOrthographicNearClip(float nearClip){m_OrthographicNear = nearClip;RecalculateProjection();}
    float GetOrthographicFarClip() const {return m_OrthographicFar;}
    void SetOrthographicFarClip(float farClip){m_OrthographicFar = farClip;RecalculateProjection();}

    ProjectionType GetProjectionType() const {return m_ProjectionType;}
    void SetProjectionType (ProjectionType type){m_ProjectionType = type;RecalculateProjection();}

private:
    void RecalculateProjection();

    float m_PerspectiveFOV = glm::radians(90.0f);
    float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;
    float m_OrthographicSize = 5.f;
    float m_OrthographicNear = -1.f,m_OrthographicFar = 5.f;


    float m_AspectRatio = 12.8/7.2;
    ProjectionType m_ProjectionType = ProjectionType::Perspective;
};}



#endif //SCENECAMERA_H
