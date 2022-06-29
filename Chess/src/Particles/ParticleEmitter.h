#pragma once

#include <iostream>
#include <vector>

#include "Particle.h"
#include "../Utils/Random.h"

class ParticleEmitter
{
public:
    ParticleEmitter();
    ParticleEmitter(Particle particle, Vector2 acceleration, float rotationAcceleration, Color startColor, Color endColor, Vector2 particleSize, Vector2 position, float lifetime, float interval, float randomness, float spread);
    void Update(float dt);
    void Render();

    void StartEmitting();
    void StopEmitting();
    void ToggleEmitting();
    bool IsEmitting();
    void EmitNow(int quantity);

    Particle GetSpawnParticle();
    void SetSpawnParticle(Particle particle);

    Vector2 GetSpawnPosition();
    void SetSpawnPosition(Vector2 position);

    Vector2 GetParticleAcceleration();
    void SetParticleAcceleration(Vector2 acceleration);

    float GetParticleRotationAcceleration();
    void SetParticleRotationAcceleration(float acceleration);
    
    float GetRandomness();
    void SetRandomness(float randomness);

    float GetSpread();
    void SetSpread(float spradValue);

    float GetParticleLifetime();
    void SetParticleLifetime(float lifetime);

    float GetSpawnInterval();
    void SetSpawnInterval(float interval);

    Color GetStartColor();
    void SetStartColor(Color color);

    Color GetEndColor();
    void SetEndColor(Color color);

    Vector2 GetParticleSize();
    void SetParticleSize(Vector2 size);

    float GetParticleCount();
    void ClearParticles();

private:
    std::vector<Particle> particles;

    float particleLifetime;
    float spawnInterval;

    float spawnTimer = 0.0f;

    Particle spawnParticle;

    Vector2 particleAcceleration;
    float particleRotationAcceleration;

    Color startColor;
    Color endColor;
    Vector2 particleSize;

    Vector2 spawnPosition;
    float randomness;
    float spread;

    bool emitting = false;
    
    Random random;
};
