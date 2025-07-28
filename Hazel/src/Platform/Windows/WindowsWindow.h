//
// Created by stang on 25-6-10.
//

#ifndef WINDOWSWINDOW_H
#define WINDOWSWINDOW_H
#include <Hazel/Core/Window.h>


struct GLFWwindow;


namespace Hazel
{
    class GraphicsContext;

    class WindowsWindow : public Window
    {
        public:
         WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();
        virtual void OnUpdate() ;

        virtual uint32_t GetWidth() const override;
        virtual uint32_t GetHeight() const override;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) override ;
        virtual void SetVSync(bool enabled) override;
        virtual bool IsVSync() const override;

        virtual void* GetNativeWindow() const override;
    private:
        void Init(const WindowProps& props);
        void Shutdown() ;

        GLFWwindow* m_window;
        Scope<GraphicsContext> m_Context;
        struct WindowData
        {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;
        };
        WindowData m_Data;
    };
}


#endif //WINDOWSWINDOW_H
