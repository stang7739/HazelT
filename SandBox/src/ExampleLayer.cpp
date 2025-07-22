//
// Created by stang on 25-7-17.
//

#include "ExampleLayer.h"
 ExampleLayer::ExampleLayer()
        : Layer("Example"), m_CameraController(1260.f / 720.f,true) // Initialize the camera with orthographic projection
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
        vertexBuffer = (Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
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
        indexBuffer=(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
        m_VertexArray->SetIndexBuffer(indexBuffer);


        auto textureshader = m_ShaderLibrary.Load("assets/shaders/triangleshader.glsl");



        // m_BlueShader.reset(Hazel::Shader::Create("assets/shaders/squareshader.glsl"));
         }

    void ExampleLayer::OnUpdate(Hazel::Timestep timestep)
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
        m_CameraController.OnUpdate(timestep);
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Hazel::RenderCommand::Clear();
        Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
        //Setting the Transparent Texture
        auto shader = m_ShaderLibrary.Get("triangleshader");
        Hazel::Renderer::SetTranform(m_squarePosition,m_squareRotation);
        shader->Bind();
        Hazel::Renderer::Submit(shader, m_VertexArray);
        Hazel::Renderer::EndScene();
    }


    void ExampleLayer::OnImGuiRender()
    {

        // ImGuiContext* ctx = ImGui::GetCurrentContext();
        // HZ_CORE_TRACE("ExampleLayer ImGui Context: {0}", (void*)ctx);
        // ImGui::Begin("SandBox");
        // glm::vec4 color = {0.2f, 0.3f, 0.8f, 1.0f};
        // ImGui::ColorEdit4("squre Color", glm::value_ptr(color));
        // ImGui::End();
    }
    void ExampleLayer::OnEvent(Hazel::Event& event)
    {
        m_CameraController.OnEvent(event);
        // HZ_CORE_TRACE("ExampleLayer Event: {0}", event);
    }