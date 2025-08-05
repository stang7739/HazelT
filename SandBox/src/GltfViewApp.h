#ifndef GLTF_VIEW_APP_H
#define GLTF_VIEW_APP_H

#include <Hazel.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GltfViewApp : public Hazel::Layer
{
public:
    GltfViewApp();
    virtual void OnUpdate(Hazel::Timestep timestep) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Hazel::Event& event) override;

private:
    Hazel::OrthographicCameraController m_CameraController;
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;
    Hazel::Ref<Hazel::Shader> m_Shader;
    Hazel::Ref<Hazel::Texture2D> m_Texture;
};

#endif // GLTF_VIEW_APP_H