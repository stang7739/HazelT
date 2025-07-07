#include <Hazel.h>
#include <glm/ext/matrix_transform.hpp>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Hazel
{
    class OpenGLShader;
}

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) // Initialize the camera with orthographic projection
    {
        // glGenVertexArrays(1, &m_VertexArray);
        // glBindVertexArray(m_VertexArray);
        m_VertexArray.reset(Hazel::VertexArray::Create());
        m_SquareVA.reset(Hazel::VertexArray::Create());

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

        float squareVertices[5 * 4] = {

            -0.5f, -0.5f, 0.0f, 0.f, 0.f,
            0.5f, -0.5f, 0.0f, 1.f, 0.f,
            0.5f, 0.5f, 0.0f, 1.f, 1.f,
            -0.5f, 0.5f, 0.0f, 0.f, 1.f
        };
        Hazel::Ref<Hazel::VertexBuffer> squarevertexBuffer;
        squarevertexBuffer.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        // HZ_CORE_TRACE("squarevertexBuffer called, ptr = {0}", (void*)squarevertexBuffer.get());


        squarevertexBuffer->SetLayout({
            {Hazel::ShaderDataType::Float3, "a_Position"}, {Hazel::ShaderDataType::Float2, "a_TexCoord"}
        });
        m_SquareVA->AddVertexBuffer(squarevertexBuffer);

        unsigned int squareIndices[6] = {
            0, 1, 2, // Triangle 1
            2, 3, 0 // Triangle 2
        };
        Hazel::Ref<Hazel::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.
            reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
        m_SquareVA->SetIndexBuffer(squareIndexBuffer);
        // HZ_CORE_TRACE("squareIndexBuffer called, ptr = {0}", (void*)squareIndexBuffer.get());

        m_Shader.reset(Hazel::Shader::Create("assets/shaders/triangleshader.glsl"));
        // m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

        std::string squarevertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;


            uniform mat4 u_ViewProjection;
            uniform mat4 u_tranform;
            out vec3 v_Position;
            out vec2 v_TexCoord;

            void main()
            {
                v_Position = a_Position;
                v_TexCoord = a_TexCoord;
              
                gl_Position = u_tranform*u_ViewProjection*vec4(a_Position, 1.0);
            }
        )";
        std::string squarefragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;
            in vec3 v_Position;
            in vec2 v_TexCoord;
            uniform sampler2D u_Texture;
            void main()
            {
                color = texture(u_Texture,v_TexCoord); // Set the color to white
            }
        )";
        m_BlueShader.reset(Hazel::Shader::Create(squarevertexSrc, squarefragmentSrc));
        m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
        m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_BlueShader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_BlueShader)->UploadUniformInt("u_Texture", 0);
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
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::D))
        {
            m_CameraRotation -= c_CameraRotationSpeed * timestep; // Move down
        }
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
            m_squareRotation -= c_CameraRotationSpeed * timestep;
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::K))
        {
            m_squareRotation += c_CameraRotationSpeed * timestep;
        }

        if (Hazel::Input::IsKeyPressed(HazelKey::I))
        {
            m_squareRotation -= c_CameraRotationSpeed * timestep;
        }
        else if (Hazel::Input::IsKeyPressed(HazelKey::K))
        {
            m_squareRotation += c_CameraRotationSpeed * timestep;
        }

        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Hazel::RenderCommand::Clear();
        Hazel::Renderer::BegeinScene(m_Camera);
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation); // Reset rotation to 0
        //Set it to a checkerboard texture first
        m_Texture->Bind(0);
        m_BlueShader->Bind();
        Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
        m_Tranform = glm::translate(glm::mat4(1.0f), {0.50f, 0.5f, 0.0f})
            * glm::rotate(glm::mat4(1.0f), glm::radians(m_squareRotation), glm::vec3(0, 0, 1))
            * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
        Hazel::Renderer::SetTranform(m_Tranform);
        //Setting the Transparent Texture
        m_ChernoLogoTexture->Bind();
        Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
        m_BlueShader->Bind();
        Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
        m_Tranform = glm::translate(glm::mat4(1.0f), m_squarePosition)
            * glm::rotate(glm::mat4(1.0f), glm::radians(m_squareRotation), glm::vec3(0, 0, 1))
            * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
        Hazel::Renderer::SetTranform(m_Tranform);

        m_Shader->Bind();
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
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;
    Hazel::Ref<Hazel::Shader> m_BlueShader;
    Hazel::Ref<Hazel::VertexArray> m_SquareVA;
    Hazel::Ref<Hazel::Shader> m_Shader;
    Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
    glm::mat4 m_Tranform = glm::mat4(1.0f); // Identity matrix
    glm::vec3 m_squarePosition = {0.0f, 0.0f, 0.0f};

    float m_CameraRotation = 0.0f; // in degrees, clockwise
    float m_squareRotation = 0.0f;
    float c_CameraMoveSpeed = 1.f;
    float c_CameraRotationSpeed = 90.f;
    float c_TranformnSpeed = 90.f;
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
