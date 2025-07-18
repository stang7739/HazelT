//
// Created by stang on 25-6-10.
//

#ifndef WINDOW_H
#define WINDOW_H
#include <complex.h>
#include <Hazel/Core/Base.h>
#include <Hazel/Events/Event.h>


namespace Hazel
{
    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowProps(const std::string& title = "Hazel Engine",
                    uint32_t width = 1600,
                    uint32_t height = 900)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    // Interface representing a desktop system based Window
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        //VSync is turned on, anti-tearing, and the rendering frame rate is limited by the screen refresh rate

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        //In order to isolate from other possible platforms (e.g. DirectX, Metal), the void* is returned uniformly.
        //Render frame rate up to ≈ screen refresh rate
        virtual void* GetNativeWindow() const = 0;

        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };
}

#endif //WINDOW_H
