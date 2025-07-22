//
// Created by stang on 25-7-18.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <Hazel_noEntityPoint.h>
#include "ParticleSystem.h"


class Player {
public:
    Player();
    void LoadAssets();
    void OnUpdate(Hazel::Timestep timestep);
    void OnRender();
    void OnImGuiRender();
    void Reset();
    float GetRotation() const { return m_Velocity.y * 4.f -90.f; }
    const glm::vec2& GetPosition() const { return m_Position; }

    uint32_t GetScore() const {return (uint32_t)(m_Position.x + 10.f) / 10.f; }

private:
    glm::vec2 m_Position = {-10.f,0.f};
    glm::vec2 m_Velocity = { 5.f, 0.f };
    Hazel::Ref<Hazel::Texture2D> m_ShipTexture;
    ParticleProps m_SmokeParticle,m_EngineParticle;
    ParticleSystem m_ParticleSystem;
    float m_Time = 0.0f;
    float m_EnginPower = 0.1f; // Engine power
    float m_Gravity = 0.4f;
    float m_SmokeEmitInterval = 0.1f; // Time interval for emitting smoke
    float m_SmokeNextEmitTime = m_SmokeEmitInterval;


};



#endif //PLAYER_H
