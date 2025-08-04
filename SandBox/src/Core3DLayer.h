#ifndef CORE3DLAYER_H
#define CORE3DLAYER_H

#include "Hazel_noEntityPoint.h"
#include <glm/glm.hpp>

namespace Hazel
{
    class Texture2D;
    class Shader;
    class VertexArray;
}

class Core3DLayer : public Hazel::Layer
{
public:
    Core3DLayer();
    virtual ~Core3DLayer() = default;
    
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Hazel::Timestep timestep) override;
    virtual void OnEvent(Hazel::Event& event) override;
    virtual void OnImGuiRender() override;

private:
    void CreateCube();
    void UpdateShaderUniforms();

    Hazel::Ref<Hazel::VertexArray> m_CubeVA;
    Hazel::Ref<Hazel::Shader> m_Core3DShader;
    Hazel::Ref<Hazel::Texture2D> m_Texture;
    
    // Camera matrices
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ModelMatrix;
    
    // CPU-controlled shader parameters
    struct ShaderParams {
        glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 viewPosition = glm::vec3(0.0f, 0.0f, 3.0f);
        
        glm::vec3 materialAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
        glm::vec3 materialDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 materialSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
        float materialShininess = 32.0f;
        
        float deformationStrength = 1.0f;  // DM parameter
        float frameworkIntensity = 0.5f;   // FW parameter
        glm::vec3 core3DColor = glm::vec3(0.5f, 0.7f, 1.0f);
    } m_ShaderParams;
    
    float m_Time = 0.0f;
    float m_CubeRotation = 0.0f;
};

#endif // CORE3DLAYER_H