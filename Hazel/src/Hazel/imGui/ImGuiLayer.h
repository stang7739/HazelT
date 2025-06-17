//
// Created by stang on 25-6-14.
//

#ifndef ImGuiLayer_H
#define ImGuiLayer_H
#include "Hazel/Core/Layer.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"


namespace Hazel{
class HAZEL_API ImGuiLayer : public Layer{
public:
    ImGuiLayer();
    ~ImGuiLayer();
    virtual void OnAttach(); //Executed when the layer is loaded into the stack
    virtual void OnDetach();//Executed when the layer is removed from the stack
    virtual void OnUpdate();//Update logic every frame
    virtual void OnEvent(Event& event);//Respond to events that are distributed by the event system

    private:

    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    bool OnMouseMoveEvent(MouseMoveEvent& e);
    bool OnMouseScrolledEvent(MouseScrolledEvent& e);
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    bool OnKeyReleasedEvent(KeyReleasedEvent& e);
    bool OnWindowResizedEvent(WindowResizeEvent& e);
    bool OnKeyTypedEvent(KeyTypedEvent& e);
    void UpdateModifierKeys();
    float m_Time = 0.0f;


};
}



#endif //ImGuiLayer_H
