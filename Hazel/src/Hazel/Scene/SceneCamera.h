//
// Created by stang on 25-8-4.
//

#ifndef SCENECAMERA_H
#define SCENECAMERA_H
#include "Hazel/Renderer/Camera.h"


namespace Hazel{
class HAZEL_API SceneCamera : public Camera {
public:
    SceneCamera();
    virtual ~SceneCamera() = default;
    void SetOrthographic(float size,float nearClip,float farclip);
    void SetViewportsize(uint32_t width,uint32_t height);
    float GetOrthographicSize() const {return m_OrthographicSize;}
    void SetOrthographicSize(float size){m_OrthographicSize = size;RecalculateProjection();}

private:
    void RecalculateProjection();
    float m_OrthographicSize = 5.f;
    float m_OrthographicNear = -1.f,m_OrthographicFar = 1.f;

    float m_AspectRatio = 12.8/7.2;

};}



#endif //SCENECAMERA_H
