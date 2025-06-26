//
// Created by stang on 25-6-7.
//

#include <hzpch.h>
#include <glad/glad.h>

#include "Application.h"
#include <Hazel/Events/ApplicationEvent.h>
#include<Hazel/Core/Log.h>

#include "imgui_internal.h"
#include "Input.h"
#include "Hazel/Events/KeyEvent.h"
#include <Hazel/Renderer/Shader.h>

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
    //This macro is used to bind the member function fn of the Application class to the current object (this)
    //and reserve a placeholder _1 to indicate that a parameter needs to be passed in when calling.

    Application* Application::s_Instance = nullptr;



    Application::Application()
    {
        HZ_CORE_ASSERT(!s_Instance, "Application already exit")
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
        m_ImGuiLayer = new ImGuiLayer{};
        PushOverlayer(m_ImGuiLayer);

        // glGenVertexArrays(1, &m_VertexArray);
        // glBindVertexArray(m_VertexArray);
    m_VertexArray.reset(VertexArray::Create());

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, // Bottom Left
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, // Bottom Right
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f // Top
        };
    std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        unsigned int indices[3] = {
            0, 1, 2 // Triangle
        };
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
        HZ_CORE_TRACE("SetIndexBuffer called, ptr = {0}", (void*)indexBuffer.get());

        m_VertexArray->SetIndexBuffer(indexBuffer);
        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            out vec3 v_Position;
            out vec4 v_Color;
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
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
        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlayer(Layer* layer)
    {
        m_LayerStack.PushOverLayer(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(Application::OnKeyPressed));
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)break;
        }
        // HZ_CORE_TRACE("{0}", e);
    }

    void Application::run()
    {
        WindowResizeEvent e(1260, 720);

        HZ_TRACE(e);

        while (m_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            auto ibo = m_VertexArray->GetIndexBuffer();
            HZ_CORE_TRACE("IndexBuffer pointer: {0}", (void*)ibo.get());


            HZ_CORE_TRACE("before shader bind");
            m_Shader->Bind();
            HZ_CORE_TRACE("after shader bind");

            HZ_CORE_TRACE("before vao bind");
            m_VertexArray->Bind();
            HZ_CORE_TRACE("after vao bind");

            HZ_CORE_TRACE("Index count: {0}", m_VertexArray->GetIndexBuffer()->GetCount());

            glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT, 0);
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                HZ_CORE_ERROR("OpenGL error: {0}", err);
            }
            HZ_CORE_ASSERT(m_VertexArray, "m_VertexArray is nullptr!");
            HZ_CORE_ASSERT(m_Shader, "m_Shader is nullptr!");
            HZ_CORE_ASSERT(m_VertexArray->GetIndexBuffer(), "IndexBuffer is nullptr!");
            HZ_CORE_TRACE("Index count: {0}", m_VertexArray->GetIndexBuffer()->GetCount());


            // for (Layer* layer : m_LayerStack)
            // {
            //     layer->OnUpdate();
            // }
            // m_ImGuiLayer->Begin();
            // for (Layer* layer : m_LayerStack)
            // {
            //     HZ_CORE_ASSERT(layer, "Layer is nullptr!");
            //     // ImGuiContext* ctx = ImGui::GetCurrentContext();
            //     // HZ_CORE_TRACE("Application ImGui Context: {0}", (void*)ctx);
            //     layer->OnImGuiRender();
            // }
            // m_ImGuiLayer->End();
            // // auto [x,y] = Input::GetMousePosition();
            // // HZ_CORE_TRACE("{0}, {1}", x, y);
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(Event& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnKeyPressed(Event& e)
    {
        KeyPressedEvent& event = (KeyPressedEvent&)e;
        if (event.GetKeyCode() == HazelKey::Escape)
        {
            m_Running = false;
            return true;
        }
        return false;
    }
}
