//
// Created by stang on 25-7-19.
//

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <Hazel_noEntityPoint.h>

struct ParticleProps
{
    glm::vec2 Position;
    glm::vec2 Velocity,VelocityVariation;
    glm::vec4 ColorBegin ,ColorEnd ;
    float SizeBegin,SizeEnd,SizeVariation;
    float LifeTime = 1.0f;
};



class ParticleSystem {
public:
    ParticleSystem();
    void Emit(const ParticleProps& particleProps);
    void OnUpdate(Hazel::Timestep timestep);
    void OnRender();
private:
    struct Particle
    {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin , ColorEnd ;
        float LifeTime = 1.0f;
        float SizeBegin, SizeEnd;
        float Rotation = 0.0f;
        float LifeRemaining = 0.f;
        bool Active = false;
    };
    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex = 1;
};



#endif //PARTICLESYSTEM_H
