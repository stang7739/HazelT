
#include <Hazel.h>

#include "Hazel/imGui/ImGuiLayer.h"

class ExampleLayer :public Hazel::Layer
{
    public:
    ExampleLayer() : Layer("Example Layer")
    {

    }
    void OnUpdate() override
    {
        // HZ_TRACE("Example Layer::OnUpdate({0})",m_DebugName);
    }
    void OnEvent(Hazel::Event& e) override
    {
        HZ_TRACE("Example Layer::OnEvent()");
    }


};

class SandBox : public Hazel::Application
{
    public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
        PushOverlayer(new Hazel::ImGuiLayer());
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