#include "GltfViewApp.h"
#include "backends/imgui_impl_opengl3.h"

GltfViewApp::GltfViewApp() : Layer("GltfView"), m_CameraController(1260.f / 720.f, true)
    {
        // Create vertex array
        m_VertexArray = (Hazel::VertexArray::Create());

        // Define a quad with texture coordinates
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom Left
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom Right
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // Top Right
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // Top Left
        };

        Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float2, "a_TexCoord"}
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0 // Two triangles forming a quad
        };
        Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
        indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        // Create texture directly with data (not loading from file)
        m_Texture = Hazel::Texture2D::Create(256, 256);
        
        // Create a simple checkerboard pattern in memory
        uint32_t* textureData = new uint32_t[256 * 256];
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                uint32_t color = ((x / 32) + (y / 32)) % 2 ? 0xFFFFFFFF : 0xFF000000;
                textureData[y * 256 + x] = color;
            }
        }
        m_Texture->setData(textureData, 256 * 256 * 4);
        delete[] textureData;

        // Load shader from file
        m_Shader = Hazel::Shader::Create("assets/shaders/GltfView.glsl");
    }

void GltfViewApp::OnUpdate(Hazel::Timestep timestep)
    {
        // Update camera
        m_CameraController.OnUpdate(timestep);

        // Render
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::Clear();

        Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

        m_Texture->Bind();
        m_Shader->Bind();
        m_Shader->SetFloat4("u_Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        Hazel::Renderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));

    }

void GltfViewApp::OnImGuiRender()
    {
        ImGui::Begin("Gltf View Settings");
        ImGui::Text("Texture created directly in code (checkerboard pattern)");
    }

void GltfViewApp::OnEvent(Hazel::Event& event)
    {
    }
}