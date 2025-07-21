//
// Created by stang on 25-7-18.
//

#include "Player.h"
#include<glm/gtc/matrix_transform.hpp>


Player::Player()
{
    //smoke
    m_SmokeParticle.Position = { 0.f, 0.f };
    m_SmokeParticle.Velocity = {-2.0f,0.0f},m_SmokeParticle.VelocityVariation = {4.f,2.f};
    m_SmokeParticle.SizeBegin = 0.35f,m_SmokeParticle.SizeEnd = 0.f;
    m_SmokeParticle.SizeVariation = 0.15f;
    m_SmokeParticle.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f };
    m_SmokeParticle.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
    m_SmokeParticle.LifeTime = 4.f;

    m_EngineParticle.Position = { 0.f, 0.f };
    m_EngineParticle.Velocity = { -2.0f, 0.0f }, m_EngineParticle.VelocityVariation = { 3.f, 1.f };
    m_EngineParticle.SizeBegin = 0.5f, m_EngineParticle.SizeEnd = 0.f;
    m_EngineParticle.SizeVariation = 0.3f;
    m_EngineParticle.ColorBegin = { 254/255.f, 109/255.f, 123/255.f, 1.0f };
    m_EngineParticle.ColorBegin = { 254/255.f, 212/255.f, 123/255.f, 1.0f };
    m_EngineParticle.LifeTime = 1.f;

}

void Player::LoadAssets()
{
    m_ShipTexture = Hazel::Texture2D::Create("assets/textures/Ship.png");
}

void Player::OnUpdate(Hazel::Timestep timestep)
{
    HZ_INFO("Player::OnUpdate");
    m_Time += timestep;
    if(Hazel::Input::IsKeyPressed(HazelKey::Space))
    {
        m_Velocity.y += m_EnginPower;
        if(m_Velocity.y < 0.f)
        {
            m_Velocity.y += m_EnginPower * 2.f;
        }
        glm::vec2 emissionPoint = {0.f,-0.6f};
        float rotation = glm::radians(GetRotation());
        glm::vec4 rotated = glm::rotate(glm::mat4(1.f),rotation,{0.f,0.f,1.f}) * glm::vec4(emissionPoint,0.f,1.f);
        m_EngineParticle.Position = m_Position + glm::vec2{rotated.x,rotated.y};
        m_EngineParticle.Velocity = -m_Velocity * 0.2f - 0.2f;
        m_ParticleSystem.Emit(m_EngineParticle);
    }else
    {
        m_Velocity.y -= m_Gravity;
    }
    m_Velocity.y = glm::clamp(m_Velocity.y, -20.f,20.f);
    m_Position += m_Velocity * (float)timestep;

    if(m_Time > m_SmokeNextEmitTime)
    {
        m_SmokeParticle.Position = m_Position;
        m_ParticleSystem.Emit(m_SmokeParticle) ;
        m_SmokeNextEmitTime += m_SmokeEmitInterval;
    }
    m_ParticleSystem.OnUpdate(timestep);
}

void Player:: OnRender()
{
    m_ParticleSystem.OnRender();
    Hazel::Renderer2D::DrawRotatedQuad({m_Position.x,m_Position.y,0.5f},{1.f,1.3f},GetRotation(),m_ShipTexture);
}

void Player::OnImGuiRender()
{
    auto bosePos = ImGui::GetWindowPos();

    ImVec2 pos = bosePos;
    pos.y += 50.f;
    std::string scoreStr = std::string("Position:") + std::to_string(m_Position.x) + ", " + std::to_string(m_Position.y);
    ImGui::GetForegroundDrawList()->AddText(nullptr, 48.f, pos, 0xffffffff, scoreStr.c_str());

    pos.y += 50.f;
    // ImGui::DragFloat2("Position", pos, 0.1f);
    // ImGui::DragFloat("Engine Power", &m_EnginPower, 0.01f, 0.f, 10.f);
     scoreStr = std::string("Engine Power:") + std::to_string(m_EnginPower) ;
    ImGui::GetForegroundDrawList()->AddText(nullptr, 48.f, pos, 0xffffffff, scoreStr.c_str());
    pos.y += 50.f;
    // ImGui::DragFloat("Gravity", &m_Gravity, 0.01f, 0.f, 10.f);
    scoreStr = std::string("Gravity:") + std::to_string(m_Gravity) ;
    ImGui::GetForegroundDrawList()->AddText(nullptr, 48.f, pos, 0xffffffff, scoreStr.c_str());


}

void Player::Reset()
{
    m_Position = {-10.f,0.f};
    m_Velocity = {5.f,0.f};

}
