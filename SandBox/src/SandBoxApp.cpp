#include <Hazel.h>
#include <glm/gtc/type_ptr.hpp>
#include "SandBox2D.h"
#include "ExampleLayer.h"


namespace Hazel
{
    class OpenGLShader;
}


class SandBox : public Hazel::Application
{
public:
    SandBox(Hazel::ApplicationCommandLineArgs args)
    {
        //https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-data Fix this problem
        ImGui::SetCurrentContext(Hazel::ImGuiLayer::GetContext());

        // PushLayer(new ExampleLayer());


        PushLayer(new SandBox2D());
        // PushLayer(new GameLayer());
    }

    ~SandBox()
    {
    }
};

//Create a SanBox Factory Pattern Abstraction of the application entry point
Hazel::Application* Hazel::CreateApplication(Hazel::ApplicationCommandLineArgs args)
{
    return new SandBox(args);
}
