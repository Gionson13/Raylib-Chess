#pragma once

#include <iostream>
#include <vector>
#include <raylib.h>

#include "../Utils/Random.h"

// TODO: Add support for textures
// TODO: Add support for multiple shapes
// TODO: Add support for spawn area

struct Particle
{
    float lifetime;

    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationVelocity;

    Color startColor;
    Color endColor;
    Vector2 size;
};

class ParticleEmitter
{
public:
    ParticleEmitter();
    ParticleEmitter(Vector2 position, Vector2 velocity, Vector2 acceleration, float centripetalAcceleration, float rotation, float rotationVelocity, float rotationAcceleration, Color startColor, Color endColor, Vector2 resolution, float minSizeFactor, float maxSizeFactor, float lifetime, float interval, float randomness, float spread);

	~ParticleEmitter();

	// Update emitter
	// @param float dt afsadfsa
    void Update(float dt);
	// Render particles
    void Render();

    void StartEmitting();
    void StopEmitting();
    void ToggleEmitting();
    bool IsEmitting() const;
    void EmitNow(int quantity);

    Vector2 GetSpawnPosition() const;
    void SetSpawnPosition(Vector2 position);

    Vector2 GetSpawnVelocity() const;
    void SetSpawnVelocity(Vector2 velocity);

    Vector2 GetParticleAcceleration() const;
    void SetParticleAcceleration(Vector2 acceleration);

    float GetCentripetalAcceleration() const;
    void SetCentripetalAcceleration(float acceleration);

    float GetParticleSpawnRotation() const;
    void SetParticleSpawnRotation(float rotation);

    float GetParticleSpawnRotationVelocity() const;
    void SetParticleSpawnRotationVelocity(float velocity);

    float GetParticleRotationAcceleration() const;
    void SetParticleRotationAcceleration(float acceleration);

    float GetRandomness() const;
    void SetRandomness(float randomness);

    float GetSpread() const;
    void SetSpread(float spradValue);

    float GetParticleLifetime() const;
    void SetParticleLifetime(float lifetime);

    float GetSpawnInterval() const;
    void SetSpawnInterval(float interval);

    Color GetStartColor() const;
    void SetStartColor(Color color);

    Color GetEndColor() const;
    void SetEndColor(Color color);

    Vector2 GetParticleResolution() const;
    void SetParticleResolution(Vector2 resolution);

    float GetParticleMinSizeFactor() const;
    void SetParticleMinSizeFactor(float factor);

    float GetParticleMaxSizeFactor() const;
    void SetParticleMaxSizeFactor(float factor);
    
    float GetParticleCount() const;
	// Remove all particles
    void ClearParticles();

private:
    std::vector<Particle> particles;

    float spawnTimer = 0.0f;

	Vector2 spawnPosition;
	Vector2 spawnVelocity;
    Vector2 particleAcceleration;

	float centripetalAcceleration;

	float spawnRotation;
    float spawnRotationVelocity;
    float particleRotationAcceleration;

    Color startColor;
    Color endColor;
    Vector2 particleResolution;
    float particleMinSizeFactor;
    float particleMaxSizeFactor;

	float lifetime;
	float spawnInterval;
    float randomness;
    float spread;

    bool emitting = false;
    
    Random random;
};
