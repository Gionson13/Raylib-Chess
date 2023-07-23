#include "ParticleEmitter.h"

#include <raymath.h>
#include "Difu/Utils/MathExtension.h"
#include "Difu/Utils/Logger.h"

#include <iostream>
#include <random>

ParticleEmitter::ParticleEmitter()
{
}


ParticleEmitter::ParticleEmitter(Vector2 position, Vector2 velocity, Vector2 acceleration, float centripetalAcceleration, float rotation, float rotationVelocity, float rotationAcceleration, Color startColor, Color endColor, Vector2 resolution, float minSizeFactor, float maxSizeFactor,  float lifetime, float interval, float randomness, float spread) :
	spawnPosition(position), spawnVelocity(velocity), particleAcceleration(acceleration),
	centripetalAcceleration(centripetalAcceleration),
	spawnRotation(rotation), spawnRotationVelocity(rotationVelocity), particleRotationAcceleration(rotationAcceleration), 
    startColor(startColor), endColor(endColor), 
    particleResolution(resolution), particleMinSizeFactor(minSizeFactor), particleMaxSizeFactor(maxSizeFactor), 
    lifetime(lifetime), spawnInterval(interval), randomness(randomness), spread(spread)
{

}


ParticleEmitter::~ParticleEmitter()
{
	ClearParticles();
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

		// Update every particle data
        particles[i].velocity += particleAcceleration * dt;
        particles[i].position += particles[i].velocity * dt;

        particles[i].rotationVelocity += particleRotationAcceleration * dt;
        particles[i].rotation += particles[i].rotationVelocity * dt;

		Vector2 perpendicularVec = { particles[i].velocity.y, -particles[i].velocity.x };
		perpendicularVec = Vector2Normalize(perpendicularVec);

		particles[i].velocity += perpendicularVec * centripetalAcceleration * dt;
    }

    
    if (emitting)
    {
		// Count how many particles should be emitted
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
        float lifeProgress = 1.0f - particle.lifetime / lifetime;
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

bool ParticleEmitter::IsEmitting() const
{
    return emitting;
}

// TODO: If spread is 0, velocity can be opposite -> wrong
void ParticleEmitter::EmitNow(int quantity)
{
    for (int i = 0; i < quantity; i++)
    {   
        Particle newParticle;
        newParticle.lifetime = lifetime;
        newParticle.position = spawnPosition;
		// Rotate velocity between -spread/2 and spread/2 from spawn velocity
        newParticle.velocity = Vector2Rotate(spawnVelocity, random.GetFloat() * spread - spread / 2.0f);
		// change velocity length based on randomness
        newParticle.velocity.x += newParticle.velocity.x * randomness * random.GetRangef(-2.0f, 0.0f);
        newParticle.velocity.y += newParticle.velocity.y * randomness * random.GetRangef(-2.0f, 0.0f);
        newParticle.rotation = spawnRotation;
        newParticle.rotationVelocity = spawnRotationVelocity + spawnRotationVelocity * randomness * random.GetRangef(-2.0f, 0.0f);
        newParticle.startColor = startColor;
        newParticle.endColor = endColor;

        float sizeFactor = random.GetRangef(particleMinSizeFactor, particleMaxSizeFactor);
        newParticle.size = {particleResolution.x * sizeFactor, particleResolution.y * sizeFactor};

        particles.emplace_back(newParticle);
    }
}

Vector2 ParticleEmitter::GetSpawnPosition() const
{
    return spawnPosition;
}

void ParticleEmitter::SetSpawnPosition(Vector2 position)
{
    spawnPosition = position;
}

Vector2 ParticleEmitter::GetSpawnVelocity() const
{
    return spawnVelocity;
}

void ParticleEmitter::SetSpawnVelocity(Vector2 velocity)
{
    spawnVelocity = velocity;
}

Vector2 ParticleEmitter::GetParticleAcceleration() const
{
    return particleAcceleration;
}

void ParticleEmitter::SetParticleAcceleration(Vector2 acceleration)
{
    particleAcceleration = acceleration;
}

float ParticleEmitter::GetCentripetalAcceleration() const
{
	return  centripetalAcceleration;
}

void ParticleEmitter::SetCentripetalAcceleration(float acceleration)
{
	centripetalAcceleration = acceleration;
}

float ParticleEmitter::GetParticleSpawnRotation() const
{
    return spawnRotation;
}

void ParticleEmitter::SetParticleSpawnRotation(float rotation)
{
	spawnRotation = rotation;
}

float ParticleEmitter::GetParticleSpawnRotationVelocity() const
{
    return spawnRotationVelocity;
}

void ParticleEmitter::SetParticleSpawnRotationVelocity(float velocity)
{
	spawnRotationVelocity = velocity;
}

float ParticleEmitter::GetParticleRotationAcceleration() const
{
    return particleRotationAcceleration;
}

void ParticleEmitter::SetParticleRotationAcceleration(float acceleration)
{
    particleRotationAcceleration = acceleration;
}

float ParticleEmitter::GetRandomness() const
{
    return randomness;
}

void ParticleEmitter::SetRandomness(float randomValue)
{
    randomness = randomValue;
}

float ParticleEmitter::GetSpread() const
{
    return spread;
}

void ParticleEmitter::SetSpread(float spradValue)
{
    spread = spradValue;
}

float ParticleEmitter::GetParticleLifetime() const
{
    return lifetime;
}

void ParticleEmitter::SetParticleLifetime(float _lifetime)
{
    lifetime = _lifetime;
}

float ParticleEmitter::GetSpawnInterval() const
{
    return spawnInterval;
}

void ParticleEmitter::SetSpawnInterval(float interval)
{
    spawnInterval = interval;
}

Color ParticleEmitter::GetStartColor() const
{
    return startColor;
}

void ParticleEmitter::SetStartColor(Color color)
{
    startColor = color;
}

Color ParticleEmitter::GetEndColor() const
{
    return endColor;
}

void ParticleEmitter::SetEndColor(Color color)
{
    endColor = color;
}

Vector2 ParticleEmitter::GetParticleResolution() const
{
    return particleResolution;
}

void ParticleEmitter::SetParticleResolution(Vector2 resolution)
{
    particleResolution = resolution;
}

float ParticleEmitter::GetParticleMinSizeFactor() const
{
    return particleMinSizeFactor;
}

void ParticleEmitter::SetParticleMinSizeFactor(float factor)
{
    particleMinSizeFactor = factor;
}

float ParticleEmitter::GetParticleMaxSizeFactor() const
{
    return particleMaxSizeFactor;
}

void ParticleEmitter::SetParticleMaxSizeFactor(float factor)
{
    particleMaxSizeFactor = factor;
}


float ParticleEmitter::GetParticleCount() const
{
    return particles.size();
}

void ParticleEmitter::ClearParticles()
{
    particles.clear();
}
