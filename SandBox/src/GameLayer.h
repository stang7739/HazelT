//
// Created by stang on 25-7-17.
//

#ifndef GAMELAYER_H
#define GAMELAYER_H
#include "Hazel_noEntityPoint.h"
#include "Level.h"


class GameLayer : public Hazel::Layer {
public:
    GameLayer();
    virtual ~GameLayer() = default;
    virtual void OnAttach() override; //Executed when the layer is loaded into the stack
    virtual void OnDetach() override; //Executed when the layer is removed from the stack
    virtual void OnUpdate(Hazel::Timestep timestep) override; //Update logic every frame
    virtual void OnEvent(Hazel::Event& event) override; //Respond to events that are distributed by the event system
    virtual void OnImGuiRender() override; //Draw the ImGui interface
    bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event);
    bool OnKeyPressed(Hazel::KeyPressedEvent& event);
    bool OnWindowResize(Hazel::WindowResizeEvent& event);


private:
    void CreateCamera(uint32_t width,uint32_t height);
    Hazel::Ref<Hazel::OrthographicCamera> m_Camera;
    // Hazel::OrthographicCamera m_camera;
    ImFont *m_Font,*m_FontSmall;
    float m_Time = 0.0f;
    bool m_Blink = false;
    Level m_Level;

    enum class GameState
    {
        Play = 0,
        MainMenu = 1,
        GameOver = 2
    };
    GameState m_State = GameState::MainMenu;
};



#endif //GAMELAYER_H
