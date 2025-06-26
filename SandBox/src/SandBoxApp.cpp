
#include <Hazel.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Hazel/Core/Input.h"


class ExampleLayer :public Hazel::Layer
{
public:
    ExampleLayer()
         : Layer("Example")
    {
    }

    void OnUpdate() override
    {

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