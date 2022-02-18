#include "generator.h"

void Generator::SetColors(Vec3f color, Vec3f dead_color, float color_randomness)
{
}

void Generator::SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
{
}

void Generator::SetMass(float mass, float mass_randomness)
{
}

int Generator::numNewParticles(float current_time, float dt) const
{
	return 0;
}

Particle* Generator::Generate(float current_time, int i)
{
	return nullptr;
}

void Generator::Paint() const
{
}

