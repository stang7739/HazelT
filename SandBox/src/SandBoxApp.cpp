#include <Hazel.h>
#include <glm/ext/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "SandBox2D.h"
#include "backends/imgui_impl_opengl3.h"

namespace Hazel
{
    class OpenGLShader;
}

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_cameraController(1260.f / 720.f,true) // Initialize the camera with orthographic projection
    {
        // glGenVertexArrays(1, &m_VertexArray);
        // glBindVertexArray(m_VertexArray);
        m_VertexArray=(Hazel::VertexArray::Create());


        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, // Bottom Right
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f // Top
        };

        Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float4, "a_Color"}
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);
        //In what order to take the vertices and draw the shape
        unsigned int indices[3] = {
            0, 1, 2 // Triangle
        };
        Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
        indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
        m_VertexArray->SetIndexBuffer(indexBuffer);


        auto textureshader = m_ShaderLibrary.Load("assets/shaders/triangleshader.glsl");



        // m_BlueShader.reset(Hazel::Shader::Create("assets/shaders/squareshader.glsl"));
         }

    void OnUpdate(Hazel::Timestep timestep) override
    {
        if (Hazel::Input::IsKeyPressed(HazelKey::J))
        {
            m_squarePosition.x -= c_CameraMoveSpeed * timestep;
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::L))
        {
            m_squarePosition.x += c_CameraMoveSpeed * timestep;
        }
        if (Hazel::Input::IsKeyPressed(HazelKey::I))
        {
            m_squareRotation -= c_RotationSpeed * timestep;
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::K))
        {
            m_squareRotation += c_RotationSpeed * timestep;
        }
        m_cameraController.OnUpdate(timestep);
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Hazel::RenderCommand::Clear();
        Hazel::Renderer::BeginScene(m_cameraController.GetCamera());
        auto c_camera = m_cameraController.GetCamera();
        // HZ_INFO("{0},{1},{2}",c_camera.GetPosition().x,c_camera.GetPosition().y,c_camera.GetPosition().z);
        //Set it to a checkerboard texture first

        //Setting the Transparent Texture
        auto shader = m_ShaderLibrary.Get("triangleshader");
        Hazel::Renderer::SetTranform(m_squarePosition,m_squareRotation);
        shader->Bind();
        Hazel::Renderer::Submit(shader, m_VertexArray);
        Hazel::Renderer::EndScene();
    }


    void OnImGuiRender() override
    {
        //https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-data Fix this problem
        ImGui::SetCurrentContext(Hazel::ImGuiLayer::GetContext());
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        // HZ_CORE_TRACE("ExampleLayer ImGui Context: {0}", (void*)ctx);
        ImGui::Begin("SandBox2D");
        glm::vec4 color = {0.2f, 0.3f, 0.8f, 1.0f};
        ImGui::ColorEdit4("squre Color", glm::value_ptr(color));
        ImGui::End();
    }
    void OnEvent(Hazel::Event& event) override
    {
        m_cameraController.OnEvent(event);
        // HZ_CORE_TRACE("ExampleLayer Event: {0}", event);
    }


private:
    Hazel::ShaderLibrary m_ShaderLibrary;
    Hazel::OrthographicCameraController m_cameraController;
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;

    Hazel::Ref<Hazel::Shader> m_Shader;
    Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;
    glm::vec3 m_squarePosition = {0.0f, 0.0f, 0.0f};
    glm::mat4 m_Tranform = glm::mat4(1.0f); // Identity matrix
    float c_RotationSpeed = 90.f;

    float c_CameraMoveSpeed = 1.f;
    float m_squareRotation = 0.0f;

};

class SandBox : public Hazel::Application
{
public:
    SandBox() : Hazel::Application()
    {
        PushLayer(new ExampleLayer());
         PushLayer(new SandBox2D());
    }

    ~SandBox()
    {
    }
};

//Create a SanBox Factory Pattern Abstraction of the application entry point
Hazel::Application* Hazel::CreateApplication()
{
    return new SandBox;
}
