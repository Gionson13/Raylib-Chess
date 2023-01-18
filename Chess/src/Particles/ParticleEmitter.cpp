#include "ParticleEmitter.h"

#include <raymath.h>
#include "../Utils/MathExtension.h"
#include "../Utils/Logger.h"

#include <iostream>
#include <random>

ParticleEmitter::ParticleEmitter()
{
}


ParticleEmitter::ParticleEmitter(Particle particle, Vector2 acceleration, float rotationAcceleration, Color startColor, Color endColor, Vector2 resolution, float minSizeFactor, float maxSizeFactor,  Vector2 position, float interval, float randomness, float spread) :
    spawnParticle(particle),
    particleAcceleration(acceleration), particleRotationAcceleration(rotationAcceleration), 
    startColor(startColor), endColor(endColor), 
    particleResolution(resolution), particleMinSizeFactor(minSizeFactor), particleMaxSizeFactor(maxSizeFactor), 
    spawnPosition(position), 
    spawnInterval(interval), randomness(randomness), spread(spread)
{

}


void ParticleEmitter::Update(float dt)
{
    for (int i = particles.size() - 1; i > -1; i--)
    {
        particles[i].lifetime -= dt;
        if (particles[i].lifetime < 0.0f)
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
        int emitCount = 0;
        while (spawnTimer > spawnInterval)
        {
            emitCount++;
            spawnTimer -= spawnInterval;
        }

        if (emitCount > 0)
            EmitNow(emitCount);
    }
}

void ParticleEmitter::Render()
{
    for (Particle particle : particles)
    {
        float lifeProgress = 1.0f - particle.lifetime / spawnParticle.lifetime;
        Color color = LerpColor(particle.startColor, particle.endColor, lifeProgress); 
        Rectangle rect = {particle.position.x, particle.position.y, particle.size.x, particle.size.y};
        DrawRectanglePro(rect, {particle.size.x / 2, particle.size.y / 2}, RAD2DEG * particle.rotation, color);
    }
}

void ParticleEmitter::StartEmitting()
{
    emitting = true;
}

void ParticleEmitter::StopEmitting()
{
    emitting = false;
}

void ParticleEmitter::ToggleEmitting()
{
    emitting = !emitting;
}

bool ParticleEmitter::IsEmitting()
{
    return emitting;
}

void ParticleEmitter::EmitNow(int quantity)
{
    for (int i = 0; i < quantity; i++)
    {   
        Particle newParticle;
        newParticle.lifetime = spawnParticle.lifetime;
        newParticle.position = spawnPosition + spawnParticle.position;
        newParticle.velocity = Vector2Rotate(spawnParticle.velocity, random.GetFloat() * spread - spread / 2.0f);
        newParticle.velocity.x += newParticle.velocity.x * randomness * random.GetRangef(-2.0f, 0.0f);
        newParticle.velocity.y += newParticle.velocity.y * randomness * random.GetRangef(-2.0f, 0.0f);
        newParticle.rotation = spawnParticle.rotation;
        newParticle.rotationVelocity = spawnParticle.rotationVelocity + spawnParticle.rotationVelocity * randomness * random.GetRangef(-2.0f, 0.0f);
        newParticle.startColor = startColor;
        newParticle.endColor = endColor;

        float sizeFactor = random.GetRangef(particleMinSizeFactor, particleMaxSizeFactor);
        newParticle.size = {particleResolution.x * sizeFactor, particleResolution.y * sizeFactor};

        particles.emplace_back(newParticle);
    }
}

Particle ParticleEmitter::GetSpawnParticle()
{
    return spawnParticle;
}

void ParticleEmitter::SetSpawnParticle(Particle particle)
{
    spawnParticle = particle;
}

Vector2 ParticleEmitter::GetSpawnPosition()
{
    return spawnPosition;
}

void ParticleEmitter::SetSpawnPosition(Vector2 position)
{
    spawnPosition = position;
}

Vector2 ParticleEmitter::GetParticleAcceleration()
{
    return particleAcceleration;
}

void ParticleEmitter::SetParticleAcceleration(Vector2 acceleration)
{
    particleAcceleration = acceleration;
}

float ParticleEmitter::GetParticleRotationAcceleration()
{
    return particleRotationAcceleration;
}

void ParticleEmitter::SetParticleRotationAcceleration(float acceleration)
{
    particleRotationAcceleration = acceleration;
}

float ParticleEmitter::GetRandomness()
{
    return randomness;
}

void ParticleEmitter::SetRandomness(float randomValue)
{
    randomness = randomValue;
}

float ParticleEmitter::GetSpread()
{
    return spread;
}

void ParticleEmitter::SetSpread(float spradValue)
{
    spread = spradValue;
}

float ParticleEmitter::GetParticleLifetime()
{
    return spawnParticle.lifetime;
}

void ParticleEmitter::SetParticleLifetime(float lifetime)
{
    spawnParticle.lifetime = lifetime;
}

float ParticleEmitter::GetSpawnInterval()
{
    return spawnInterval;
}

void ParticleEmitter::SetSpawnInterval(float interval)
{
    spawnInterval = interval;
}

Color ParticleEmitter::GetStartColor()
{
    return startColor;
}

void ParticleEmitter::SetStartColor(Color color)
{
    startColor = color;
}

Color ParticleEmitter::GetEndColor()
{
    return endColor;
}
void ParticleEmitter::SetEndColor(Color color)
{
    endColor = color;
}

Vector2 ParticleEmitter::GetParticleResolution()
{
    return particleResolution;
}

void ParticleEmitter::SetParticleResolution(Vector2 resolution)
{
    particleResolution = resolution;
}

float ParticleEmitter::GetParticleMinSizeFactor()
{
    return particleMinSizeFactor;
}

void ParticleEmitter::SetParticleMinSizeFactor(float factor)
{
    particleMinSizeFactor = factor;
}

float ParticleEmitter::GetParticleMaxSizeFactor()
{
    return particleMaxSizeFactor;
}

void ParticleEmitter::SetParticleMaxSizeFactor(float factor)
{
    particleMaxSizeFactor = factor;
}


float ParticleEmitter::GetParticleCount()
{
    return particles.size();
}

void ParticleEmitter::ClearParticles()
{
    particles.clear();
}
