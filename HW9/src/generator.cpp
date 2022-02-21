#include "generator.h"
#include "windows.h"
#include <gl/glut.h>

void Generator::SetColors(const Vec3f& color, const Vec3f& dead_color, float color_randomness)
{
	this->color = color;
	this->dead_color = dead_color;
	this->color_randomness = color_randomness;
}

void Generator::SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
{
	this->lifespan = lifespan;
	this->lifespan_randomness = lifespan_randomness;
	this->desired_num_particles = desired_num_particles;
}

void Generator::SetMass(float mass, float mass_randomness)
{
	this->mass = mass;
	this->mass_randomness = mass_randomness;
}


int HoseGenerator::numNewParticles(float current_time, float dt) const
{
	return dt * desired_num_particles / lifespan;
}

Particle* HoseGenerator::Generate(float current_time, int i)
{
	Vec3f p = pos + pos_randomness * random->randomVector();
	Vec3f velocity = v + v_randomness * random->randomVector();
	Vec3f c = color + color_randomness * random->randomVector();
	Vec3f dc = dead_color + color_randomness * random->randomVector();
	float ls = lifespan + lifespan_randomness * random->next();
	float m = mass + mass_randomness * random->next();
	Particle* particle = new Particle(p, velocity, c, dc, m, ls);
	return particle;
}

int RingGenerator::numNewParticles(float current_time, float dt) const
{
	return (current_time < 3 ? current_time / 3 : 1) * desired_num_particles;
}

Particle* RingGenerator::Generate(float current_time, int i)
{
	float radius = current_time < 3 ? current_time : 3;
	Vec3f p(radius * sin(i) + pos_randomness * random->next(), -4, radius * cos(i) + pos_randomness * random->next());
	Vec3f velocity = v + v_randomness * random->randomVector();
	Vec3f c = color + color_randomness * random->randomVector();
	Vec3f dc = dead_color + color_randomness * random->randomVector();
	float ls = lifespan + lifespan_randomness * random->next();
	float m = mass + mass_randomness * random->next();
	Particle* particle = new Particle(p, velocity, c, dc, m, ls);
	return particle;
}

void RingGenerator::Paint() const {
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_QUADS);
	glVertex3f(5, -4, 5);
	glVertex3f(5, -4, -5);
	glVertex3f(-5, -4, -5);
	glVertex3f(-5, -4, 5);
	glEnd();
}
