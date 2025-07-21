//
// Created by stang on 25-7-18.
//

#ifndef LEVEL_H
#define LEVEL_H
#include <Hazel_noEntityPoint.h>
#include <glm/vec3.hpp>

#include "Player.h"

struct Pillar
{
    glm::vec3 TopPosition = {0.f,10.f,0.f};
    glm::vec2 TopScale = {15.f,20.f};

    glm::vec3 BottomPosition = {10.f,10.f,0.f};
    glm::vec2 BottomScale = {15.f,20.f};
};

namespace Hazel
{
    class Timestep;
}

class Level {
public:
    void Init();
    void OnUpdate(Hazel::Timestep timestep);
    void OnRender();
    void OnImGuiRender();
    bool IsGameOver();
    void Reset();
    Player& GetPlayer() { return m_Player; }

private:
    void CreatePillar(int index, float offset);
    bool CollisionTest();
    void GameOver();
    Player m_Player;
    bool m_GameOver = false;
    float m_PillarTarget = 30.f;
    int m_PillarIndex = 0;
    glm::vec3 m_PillarHSV = { 0.0f, 0.8f, 0.8f };
    std::vector<Pillar> m_Pillars;

    Hazel::Ref<Hazel::Texture2D> m_TriangleTexture;

};



#endif //LEVEL_H
