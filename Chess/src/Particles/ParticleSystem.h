#pragma once

#include <iostream>
#include <vector>

#include "Particle.h"

class ParticleSystem
{
public:
    ParticleSystem();
    ParticleSystem(Particle particle, Vector2 position, float lifetime, float interval, float randomness, float spread);
    void Update(float dt);
    void Render();

    void StartEmitting();
    void StopEmitting();
    void ToggleEmitting();
    bool IsEmitting();

    Particle GetSpawnParticle();
    void SetSpawnParticle(Particle particle);

    Vector2 GetSpawnPosition();
    void SetSpawnPosition(Vector2 position);

    float GetRandomness();
    void SetRandomness(float randomness);

    float GetSpread();
    void SetSpread(float spradValue);

    float GetParticleLifetime();
    void SetParticleLifetime(float lifetime);

    float GetSpawnInterval();
    void SetSpawnInterval(float interval);

    void ClearParticles();

private:
    std::vector<Particle> particles;

    float particleLifetime;
    float spawnInterval;

    float spawnTimer = 0.0f;

    Particle spawnParticle;
    Vector2 spawnPosition;
    float randomness;
    float spread;

    bool emitting = false;

};
