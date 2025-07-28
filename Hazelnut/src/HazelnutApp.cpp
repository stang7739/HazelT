#include <Hazel.h>
#include <glm/ext/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

#include "backends/imgui_impl_opengl3.h"

namespace Hazel
{
    class OpenGLShader;
}


class Hazelnet : public Hazel::Application
{
public:
    Hazelnet() : Hazel::Application("Hazelnut")
    {
        //https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-data Fix this problem
        ImGui::SetCurrentContext(Hazel::ImGuiLayer::GetContext());

        // PushLayer(new ExampleLayer());


        PushLayer(new EditorLayer());
        // PushLayer(new GameLayer());
    }

    ~Hazelnet()
    {
    }
};

//Create a SanBox Factory Pattern Abstraction of the application entry point
Hazel::Application* Hazel::CreateApplication()
{
    return new Hazelnet;
}
