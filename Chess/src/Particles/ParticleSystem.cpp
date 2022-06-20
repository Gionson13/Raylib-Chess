#include "ParticleSystem.h"

#include <raymath.h>
#include "../Utils/MathExtension.h"
#include "../Utils/Logger.h"

#include <iostream>
#include <random>

ParticleSystem::ParticleSystem()
{
    
}


ParticleSystem::ParticleSystem(Particle particle, Vector2 acceleration, float rotationAcceleration, Color startColor, Color endColor, Vector2 particleSize,  Vector2 position, float lifetime, float interval, float randomness, float spread) : 
    spawnParticle(particle), particleAcceleration(acceleration), particleRotationAcceleration(rotationAcceleration), startColor(startColor), endColor(endColor), particleSize(particleSize), spawnPosition(position), particleLifetime(lifetime), spawnInterval(interval), randomness(randomness), spread(spread)
{

}


void ParticleSystem::Update(float dt)
{
    for (int i = particles.size() - 1; i > -1; i--)
    {
        particles[i].life += dt;
        if (particles[i].life > particleLifetime)
        {
            particles.erase(particles.begin() + i);
            continue;
        }

        particles[i].velocity += particleAcceleration * dt;
        particles[i].position += particles[i].velocity * dt;

        particles[i].rotationVelocity += particleRotationAcceleration * dt;
        particles[i].rotation += particles[i].rotationVelocity * dt;
    }

    
    if (emitting)
    {
        spawnTimer += dt;
        while (spawnTimer > spawnInterval)
        {
            Particle newParticle;
            newParticle.position = spawnPosition + spawnParticle.position;
            newParticle.velocity = Vector2Rotate(spawnParticle.velocity, random.GetFloat() * spread - spread / 2.0f);
            newParticle.velocity.x += newParticle.velocity.x * randomness * random.GetRangef(-2.0f, 0.0f);
            newParticle.velocity.y += newParticle.velocity.y * randomness * random.GetRangef(-2.0f, 0.0f);
            newParticle.rotation = spawnParticle.rotation;
            newParticle.rotationVelocity = spawnParticle.rotationVelocity + spawnParticle.rotationVelocity * randomness * random.GetRangef(-2.0f, 0.0f);
            particles.emplace_back(newParticle);

            spawnTimer -= spawnInterval;
        }
    }
}

void ParticleSystem::Render()
{
    for (Particle particle : particles)
    {
        float lifeProgress = particle.life / particleLifetime;
        Rectangle rect = {particle.position.x, particle.position.y, particleSize.x, particleSize.y};
        Color color = LerpColor(startColor, endColor, lifeProgress); 
        DrawRectanglePro(rect, {particleSize.x / 2, particleSize.y / 2}, RAD2DEG * particle.rotation, color);
    }
}

void ParticleSystem::StartEmitting()
{
    emitting = true;
}

void ParticleSystem::StopEmitting()
{
    emitting = false;
}

void ParticleSystem::ToggleEmitting()
{
    emitting = !emitting;
}

bool ParticleSystem::IsEmitting()
{
    return emitting;
}

Particle ParticleSystem::GetSpawnParticle()
{
    return spawnParticle;
}

void ParticleSystem::SetSpawnParticle(Particle particle)
{
    spawnParticle = particle;
}

Vector2 ParticleSystem::GetSpawnPosition()
{
    return spawnPosition;
}

void ParticleSystem::SetSpawnPosition(Vector2 position)
{
    spawnPosition = position;
}

Vector2 ParticleSystem::GetParticleAcceleration()
{
    return particleAcceleration;
}

void ParticleSystem::SetParticleAcceleration(Vector2 acceleration)
{
    particleAcceleration = acceleration;
}

float ParticleSystem::GetParticleRotationAcceleration()
{
    return particleRotationAcceleration;
}

void ParticleSystem::SetParticleRotationAcceleration(float acceleration)
{
    particleRotationAcceleration = acceleration;
}

float ParticleSystem::GetRandomness()
{
    return randomness;
}

void ParticleSystem::SetRandomness(float randomValue)
{
    randomness = randomValue;
}

float ParticleSystem::GetSpread()
{
    return spread;
}

void ParticleSystem::SetSpread(float spradValue)
{
    spread = spradValue;
}

float ParticleSystem::GetParticleLifetime()
{
    return particleLifetime;
}

void ParticleSystem::SetParticleLifetime(float lifetime)
{
    particleLifetime = lifetime;
}

float ParticleSystem::GetSpawnInterval()
{
    return spawnInterval;
}

void ParticleSystem::SetSpawnInterval(float interval)
{
    spawnInterval = interval;
}

Color ParticleSystem::GetStartColor()
{
    return startColor;
}

void ParticleSystem::SetStartColor(Color color)
{
    startColor = color;
}

Color ParticleSystem::GetEndColor()
{
    return endColor;
}
void ParticleSystem::SetEndColor(Color color)
{
    endColor = color;
}

Vector2 ParticleSystem::GetParticleSize()
{
    return particleSize;
}

void ParticleSystem::SetParticleSize(Vector2 size)
{
    particleSize = size;
}

void ParticleSystem::ClearParticles()
{
    particles.clear();
}
