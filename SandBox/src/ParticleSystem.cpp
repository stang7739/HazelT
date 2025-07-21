//
// Created by stang on 25-7-19.
//

#include "ParticleSystem.h"

#include <glm/ext/quaternion_common.hpp>

#include "Random.h"


ParticleSystem::ParticleSystem()
{
    m_ParticlePool.resize(1000);
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
    Particle& particle = m_ParticlePool[m_PoolIndex];
    particle.Active = true;
    particle.Position = particleProps.Position;

    //Velocity
    particle.Velocity = particleProps.Velocity;
    particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
    particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

    //color
    particle.ColorBegin = particleProps.ColorBegin;
    particle.ColorEnd = particleProps.ColorEnd;

    //Size
    particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
    particle.SizeEnd = particleProps.SizeEnd ;

    //Life
    particle.LifeTime = particleProps.LifeTime;
    particle.LifeRemaining = particleProps.LifeTime;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();

}

void ParticleSystem::OnUpdate(Hazel::Timestep timestep)
{
    HZ_INFO("ParticleSystem::OnUpdate");
    for(auto& particle :m_ParticlePool)
    {
        if(!particle.Active)
            continue;
        if(particle.LifeRemaining <= 0.f)
        {
            particle.Active = false;
            continue;
        }

        particle.LifeRemaining -= timestep;
        particle.Position += particle.Velocity * (float)timestep;
        particle.Rotation += 0.01f * (float)timestep; // Rotate the particle slightly
    }
}

void ParticleSystem::OnRender()
{
    for (auto& particle : m_ParticlePool)
    {
        if (!particle.Active)
            continue;
        float life = particle.LifeRemaining / particle.LifeTime;
        // glm::lerp(a, b, t) represents linear interpolation between a and b in proportional t (a for t=0 and b for t=1).
        glm::vec4 color = glm::mix(particle.ColorEnd, particle.ColorBegin, life);
        color.a = color.a * life;

        float size = glm::mix(particle.SizeEnd,particle.SizeBegin,life);
        Hazel::Renderer2D::DrawRotatedQuad(particle.Position,{size,size},particle.Rotation,color);
    }
}
