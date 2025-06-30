#include <Hazel.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Renderer/Buffer.h"


class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) // Initialize the camera with orthographic projection
    {
        // glGenVertexArrays(1, &m_VertexArray);
        // glBindVertexArray(m_VertexArray);
        m_VertexArray.reset(Hazel::VertexArray::Create());

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, // Bottom Left
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, // Bottom Right
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f // Top
        };
        std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float4, "a_Color"}
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        unsigned int indices[3] = {
            0, 1, 2 // Triangle
        };
        std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
        indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
        HZ_CORE_TRACE("SetIndexBuffer called, ptr = {0}", (void*)indexBuffer.get());

        m_VertexArray->SetIndexBuffer(indexBuffer);
        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            uniform mat4 u_ViewProjection;
            out vec3 v_Position;
            out vec4 v_Color;
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection*vec4(a_Position, 1.0);
            }
        )";
        std::string fragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            in vec4 v_Color;
            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5,1.0); // Set the color to white
                color = v_Color;
            }
        )";
        m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));
    }

    void OnUpdate(Hazel::Timestep timestep) override
    {

        if (Hazel::Input::IsKeyPressed(HazelKey::LeftArrow))
        {
            m_CameraPosition.x -= c_CameraMoveSpeed * timestep; // Move left
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::RightArrow))
        {
            m_CameraPosition.x += c_CameraMoveSpeed * timestep; // Move right
        }
        if (Hazel::Input::IsKeyPressed(HazelKey::UpArrow))
        {
            m_CameraPosition.y += c_CameraMoveSpeed * timestep; // Move up
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::DownArrow))
        {
            m_CameraPosition.y -= c_CameraMoveSpeed * timestep; // Move down
        }

        if (Hazel::Input::IsKeyPressed(HazelKey::A))
        {
            m_CameraRotation += c_CameraRotationSpeed * timestep; // Move up
            HZ_INFO((float)m_CameraRotation);
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::D))
        {
            m_CameraRotation -= c_CameraRotationSpeed * timestep; // Move down
        }

        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Hazel::RenderCommand::Clear();
        Hazel::Renderer::BegeinScene(m_Camera);
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation); // Reset rotation to 0
        Hazel::Renderer::Submit(m_Shader, m_VertexArray);
        Hazel::Renderer::EndScene();
    }


    void OnImGuiRender() override
    {
        //https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-data Fix this problem
        ImGui::SetCurrentContext(Hazel::ImGuiLayer::GetContext());
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        // HZ_CORE_TRACE("ExampleLayer ImGui Context: {0}", (void*)ctx);
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    }

private:
    Hazel::OrthographicCamera m_Camera;
    std::shared_ptr<Hazel::VertexArray> m_VertexArray;
    std::shared_ptr<Hazel::Shader> m_BlueShader;
    std::shared_ptr<Hazel::VertexArray> m_SquareVA;
    std::shared_ptr<Hazel::Shader> m_Shader;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
    float m_CameraRotation = 0.0f; // in degrees, clockwise
    float c_CameraMoveSpeed = 1.f;
    float c_CameraRotationSpeed = 90.f;
};

class SandBox : public Hazel::Application
{
public:
    SandBox() : Hazel::Application()
    {
        PushLayer(new ExampleLayer());
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
