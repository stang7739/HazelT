//
// Created by stang on 25-6-7.
//

#ifndef Application_H
#define Application_H
#include <Hazel/Core/Core.h>
#include <Hazel/Events/Event.h>

#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"
#include "Hazel/imGui/ImGuiLayer.h"
#include "Hazel/Renderer/OrthographicCamera.h"

namespace Hazel
{
    class VertexArray;
    class IndexBuffer;
}

namespace Hazel
{
    class VertexBuffer;
}

namespace Hazel
{
    struct ApplicationCommandLineArgs
    {
        int Count=0;
        char** Args = nullptr;
        const char* operator[](int index) const
        {
            // HZ_CORE_ASSERT("{}",index < Count);
            return Args[index];
        }
    };
    struct ApplicationSpecification
    {
        std::string Name = "Hazel Application";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };
    class Shader;

    // Symbolic macros everywhere
    class HAZEL_API Application
    {

    public:
        Application(const ApplicationSpecification& specification);
        //Virtual constructor function:
        //Support class polymorphism Ensure that the derived class object created by the base pointer
        //calls the corresponding destructor to properly free up memory
        virtual ~Application();
        void run();
        void PushLayer(Layer* layer);
        void PushOverlayer(Layer* layer);
        void OnEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        inline Window& GetWindow( ){return *m_Window;}
        void Close();
        inline static Application& Get(){return *s_Instance;}
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
        const ApplicationSpecification& GetSpecifition() const { return m_Specification; }

    private:
        //s_Instance is a static pointer member variable of the Application class
        //It is declared within the class, defined outside the class, and globally unique
        //The address used to store this unique application instance
        static Application* s_Instance;
        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
        float m_LastFrameTime = 0.0f;
        bool m_Minimized = false;
        ApplicationCommandLineArgs m_CommandLineArgs;
        ApplicationSpecification m_Specification;
    };

    // Application* CreateApplication();
    Application* CreateApplication(ApplicationCommandLineArgs args);
}
#endif //Application_H
