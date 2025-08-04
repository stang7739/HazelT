#include "Core3DLayer.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Core3DLayer::Core3DLayer()
    : Layer("Core3DLayer")
{
    // Initialize projection matrix
    m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    // Initialize view matrix
    m_ViewMatrix = glm::lookAt(
        m_ShaderParams.viewPosition,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    // Initialize model matrix
    m_ModelMatrix = glm::mat4(1.0f);
}

void Core3DLayer::OnAttach()
{
    HZ_PROFILE_FUNCTION();
    
    // Create cube geometry
    CreateCube();
    
    // Load the shader with CPU-controlled parameters
    m_Core3DShader = Hazel::Shader::Create("assets/shaders/Core3D_DM_FW.glsl");
    
    // Create a simple white texture
    m_Texture = Hazel::Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    m_Texture->setData(&whiteTextureData, sizeof(uint32_t));
}

void Core3DLayer::OnDetach()
{
    HZ_PROFILE_FUNCTION();
}

void Core3DLayer::OnUpdate(Hazel::Timestep timestep)
{
    HZ_PROFILE_FUNCTION();
    
    m_Time += timestep;
    m_CubeRotation += timestep * 50.0f; // Rotate 50 degrees per second
    
    // Update model matrix with rotation
    m_ModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_CubeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
    
    // Clear the screen
    Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Hazel::RenderCommand::Clear();
    
    // Bind shader and update uniforms from CPU
    m_Core3DShader->Bind();
    UpdateShaderUniforms();
    
    // Bind texture
    m_Texture->Bind();
    
    // Render the cube
    Hazel::Renderer::Submit(m_Core3DShader, m_CubeVA);
}

void Core3DLayer::OnEvent(Hazel::Event& event)
{
    HZ_PROFILE_FUNCTION();
}

void Core3DLayer::OnImGuiRender()
{
    HZ_PROFILE_FUNCTION();
    
    ImGui::Begin("Core3D CPU Controls");
    
    ImGui::Text("This demonstrates CPU-side control instead of hardcoded shader values");
    ImGui::Separator();
    
    // Light controls
    if (ImGui::CollapsingHeader("Lighting"))
    {
        ImGui::SliderFloat3("Light Position", glm::value_ptr(m_ShaderParams.lightPosition), -5.0f, 5.0f);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(m_ShaderParams.lightColor));
        ImGui::SliderFloat3("View Position", glm::value_ptr(m_ShaderParams.viewPosition), -5.0f, 5.0f);
    }
    
    // Material controls
    if (ImGui::CollapsingHeader("Material"))
    {
        ImGui::ColorEdit3("Ambient", glm::value_ptr(m_ShaderParams.materialAmbient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(m_ShaderParams.materialDiffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(m_ShaderParams.materialSpecular));
        ImGui::SliderFloat("Shininess", &m_ShaderParams.materialShininess, 1.0f, 128.0f);
    }
    
    // Core3D specific controls (DM/FW parameters)
    if (ImGui::CollapsingHeader("Core3D Effects"))
    {
        ImGui::SliderFloat("Deformation Strength (DM)", &m_ShaderParams.deformationStrength, 0.0f, 5.0f);
        ImGui::SliderFloat("Framework Intensity (FW)", &m_ShaderParams.frameworkIntensity, 0.0f, 2.0f);
        ImGui::ColorEdit3("Core3D Color", glm::value_ptr(m_ShaderParams.core3DColor));
    }
    
    ImGui::Text("All parameters are controlled from CPU via uniforms!");
    ImGui::Text("No hardcoded values in core3d_dm_fw.frag");
    
    ImGui::End();
}

void Core3DLayer::CreateCube()
{
    // Create cube vertices with positions, normals, and texture coordinates
    float vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        
        // Right face
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        
        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f
    };
    
    uint32_t indices[] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Top face
        8, 9, 10, 10, 11, 8,
        // Bottom face
        12, 13, 14, 14, 15, 12,
        // Right face
        16, 17, 18, 18, 19, 16,
        // Left face
        20, 21, 22, 22, 23, 20
    };
    
    m_CubeVA = Hazel::VertexArray::Create();
    
    Hazel::Ref<Hazel::VertexBuffer> vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->SetLayout({
        {Hazel::ShaderDataType::Float3, "a_Position"},
        {Hazel::ShaderDataType::Float3, "a_Normal"},
        {Hazel::ShaderDataType::Float2, "a_TexCoord"}
    });
    
    m_CubeVA->AddVertexBuffer(vertexBuffer);
    
    Hazel::Ref<Hazel::IndexBuffer> indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_CubeVA->SetIndexBuffer(indexBuffer);
}

void Core3DLayer::UpdateShaderUniforms()
{
    // Update view matrix based on current view position
    m_ViewMatrix = glm::lookAt(
        m_ShaderParams.viewPosition,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    // Calculate normal matrix
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_ModelMatrix)));
    
    // Set matrix uniforms
    m_Core3DShader->SetMat4("u_Model", m_ModelMatrix);
    m_Core3DShader->SetMat4("u_View", m_ViewMatrix);
    m_Core3DShader->SetMat4("u_Projection", m_ProjectionMatrix);
    m_Core3DShader->SetMat3("u_NormalMatrix", normalMatrix);
    
    // Set lighting uniforms - ALL CONTROLLED FROM CPU!
    m_Core3DShader->SetFloat3("u_LightPosition", m_ShaderParams.lightPosition);
    m_Core3DShader->SetFloat3("u_LightColor", m_ShaderParams.lightColor);
    m_Core3DShader->SetFloat3("u_ViewPosition", m_ShaderParams.viewPosition);
    
    // Set material uniforms - ALL CONTROLLED FROM CPU!
    m_Core3DShader->SetFloat3("u_MaterialAmbient", m_ShaderParams.materialAmbient);
    m_Core3DShader->SetFloat3("u_MaterialDiffuse", m_ShaderParams.materialDiffuse);
    m_Core3DShader->SetFloat3("u_MaterialSpecular", m_ShaderParams.materialSpecular);
    m_Core3DShader->SetFloat("u_MaterialShininess", m_ShaderParams.materialShininess);
    
    // Set Core3D specific uniforms - DM/FW parameters controlled from CPU!
    m_Core3DShader->SetFloat("u_DeformationStrength", m_ShaderParams.deformationStrength);
    m_Core3DShader->SetFloat("u_FrameworkIntensity", m_ShaderParams.frameworkIntensity);
    m_Core3DShader->SetFloat3("u_Core3DColor", m_ShaderParams.core3DColor);
    
    // Set texture uniform
    m_Core3DShader->SetInt("u_Texture", 0);
}