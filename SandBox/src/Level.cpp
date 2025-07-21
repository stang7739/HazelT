//
// Created by stang on 25-7-18.
//

#include "Level.h"
#include<glm/gtc/matrix_transform.hpp>

#include "Random.h"

static glm::vec4 HSVtoRGB(const glm::vec3& hsv)
{
    int H = (int)(hsv.x * 360.f);
    double S = hsv.y;
    double V = hsv.z;

    double C = V * S;
    double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs, Gs, Bs;
    if (H >= 0 && H < 60)
    {
        Rs = C;
        Gs = X;
        Bs = 0;
    }
    else if (H >= 60 && H < 120)
    {
        Rs = X;
        Gs = C;
        Bs = 0;
    }
    else if (H >= 120 && H < 180)
    {
        Rs = 0;
        Gs = C;
        Bs = X;
    }
    else if (H >= 180 && H < 240)
    {
        Rs = 0;
        Gs = X;
        Bs = C;
    }
    else if (H >= 240 && H < 300)
    {
        Rs = X;
        Gs = 0;
        Bs = C;
    }
    else
    {
        Rs = C;
        Gs = 0;
        Bs = X;
    }
    return glm::vec4(Rs + m, Gs + m, Bs + m, 1.0f);
}

static bool PointInTri(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
    float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
    float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

    if ((s < 0) != (t < 0))
    {
        return false;
    }
    float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

    return A < 0 ? (s <= 0 && s + t >= A) : (s >= 0 && s + t <= A);
}

void Level::Init()
{
    m_TriangleTexture = Hazel::Texture2D::Create("assets/textures/triangle.png");
    m_Player.LoadAssets();
    m_Pillars.resize(5);
    for (int i = 0; i < m_Pillars.size(); ++i)
    {
        CreatePillar(i, i * 10.f);
    }
}

void Level::OnRender()
{
    const auto& playerPos = m_Player.GetPosition();
    glm::vec4 color = HSVtoRGB(m_PillarHSV);

    Hazel::Renderer2D::DrawQuad({playerPos.x, 0.0f, -0.8f}, {50.f, 50.f}, {0.3f, 0.3f, 0.3f, 1.0f});
    // Hazel::Renderer2D::DrawQuad({-10.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});

    //Floor and ceiling
    Hazel::Renderer2D::DrawQuad({playerPos.x, 34.f}, {50.f, 50.f}, color);
    Hazel::Renderer2D::DrawQuad({playerPos.x, -34.f}, {50.f, 50.f}, color);

    for (auto& pillar : m_Pillars)
    {
        Hazel::Renderer2D::DrawRotatedQuad(pillar.TopPosition, pillar.TopScale, 180.f, m_TriangleTexture, 1.f, color);
        Hazel::Renderer2D::DrawRotatedQuad(pillar.BottomPosition, pillar.BottomScale, 0.f, m_TriangleTexture, 1.f,
                                           color);
    }
    m_Player.OnRender();
}

void Level::OnUpdate(Hazel::Timestep timestep)
{
    m_Player.OnUpdate(timestep);
    if (CollisionTest())
    {
        GameOver();
        return;
    }
    m_PillarHSV.x += 0.1f * timestep;
    if (m_PillarHSV.x > 1.f)
        m_PillarHSV.x = 0.f;
    if (m_Player.GetPosition().x > m_PillarTarget)
    {
        CreatePillar(m_PillarIndex, m_PillarTarget + 20.f);
        m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
        m_PillarTarget += 10.f;
    }
}

void Level::OnImGuiRender()
{
    m_Player.OnImGuiRender();
}

bool Level::IsGameOver()
{
    return m_GameOver;
}


void Level::CreatePillar(int index, float offset)
{
    Pillar& pillar = m_Pillars[index];
    pillar.TopPosition.x = offset;
    pillar.BottomPosition.x = offset;
    pillar.TopPosition.z = index * 0.1f - 0.5f;
    pillar.BottomPosition.z = index * 0.1f - 0.5f + 0.05f;

    float center = Random::Float() * 35.f - 17.5f;
    float gap = 2.0f + Random::Float() * 5.f;
    pillar.TopPosition.y = 2.f - ((10.f - center) * 0.2f) + 5.f * gap;
    pillar.BottomPosition.y = -2.f - ((-10.f - center) * 0.2f) - 5.f * gap;
}

bool Level::CollisionTest()
{
    if (glm::abs(m_Player.GetPosition().y) > 8.5f)
    {
        return true;
    }
    glm::vec4 playVertices[4] = {
        {-0.5f, -0.5f, 0.0f, 1.f},
        {0.5f, -0.5f, 0.0f, 1.f},
        {0.5f, 0.5f, 0.0f, 1.f},
        {-0.5f, 0.5f, 0.0f, 1.f}
    };
    const auto& pos = m_Player.GetPosition();
    glm::vec4 playerTransformedVerts[4];
    for (auto i = 0; i < 4; i++)
    {
        playerTransformedVerts[i] = glm::translate(glm::mat4(1.f), {pos.x, pos.y, 0.f})
            * glm::rotate(glm::mat4(1.f), glm::radians(m_Player.GetRotation()), {0.f, 0.f, 1.f})
            * glm::scale(glm::mat4(1.f), {1.f, 1.3f, 1.f}) * playVertices[i];
    }
    glm::vec4 pillarVertices[3] = {
        {-0.5f + 0.1f, -0.5 + 0.1f, 0.f, 1.f},
        {0.5f - 0.1f, -0.5 + 0.1f, 0.f, 1.f},
        {0.0f + 0.0f, 0.5 - 0.1f, 0.f, 1.f},
    };
    for (auto& p : m_Pillars)
    {
        glm::vec2 tri[3];
        for (int i = 0; i < 3; i++)
        {
            tri[i] = glm::translate(glm::mat4(1.f), {p.TopPosition.x, p.TopPosition.y, 0.f})
                * glm::rotate(glm::mat4(1.f), glm::radians(180.f), {0.f, 0.f, 1.f})
                * glm::scale(glm::mat4(1.f), {p.TopScale.x, p.TopScale.y, 1.f})
                * pillarVertices[i];
        }
        for (auto& vert : playerTransformedVerts)
        {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2]))
            {
                return true;
            }
        }
        for (int i = 0; i < 3; i++)
        {
            tri[i] = glm::translate(glm::mat4(1.f), {p.BottomPosition.x, p.BottomPosition.y, 0.f})
                * glm::rotate(glm::mat4(1.f), glm::radians(180.f), {0.f, 0.f, 1.f})
                * glm::scale(glm::mat4(1.f), {p.BottomScale.x, p.BottomScale.y, 1.f})
                * pillarVertices[i];
        }
        for (auto& vert : playerTransformedVerts)
        {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2]))
            {
                return true;
            }
        }
    }
    return false;
}

void Level::Reset()
{
    m_GameOver = false;
    m_Player.Reset();
    m_PillarTarget = 30.f;
    m_PillarIndex = 0;
    for (int i = 0; i < m_Pillars.size(); ++i)
    {
        CreatePillar(i, i * 10.f);
    }
}

void Level::GameOver()
{
    m_GameOver = true;
}
