#pragma once
#include "glCanvas.h"
#include "matrix.h"
#include "parser.h"
#include "particle.h"
#include "particleSet.h"
#include "random.h"
#include "system.h"
#include "vectors.h"
#include "forcefield.h"
#include "integrator.h"
#include "random.h"

class Generator {
public:
    Generator() { random = new Random(); }

    virtual ~Generator() { delete random; }

    // for the gui
    virtual void Paint() const {}

    void Restart() { delete random; random = new Random; }

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const = 0;

    virtual Particle* Generate(float current_time, int i) = 0;

    // initialization
    void SetColors(const Vec3f& color, const Vec3f& dead_color, float color_randomness);

    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles);

    void SetMass(float mass, float mass_randomness);
    

protected:
    Vec3f color;
    Vec3f dead_color;
    float color_randomness;
    float lifespan;
    float lifespan_randomness;
    int desired_num_particles;
    float mass;
    float mass_randomness;
    Random* random;
};


class HoseGenerator :public Generator {
public:
	
	HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness)
		:pos(position), pos_randomness(position_randomness), v(velocity), v_randomness(velocity_randomness) {};

	int numNewParticles(float current_time, float dt) const override;

	Particle* Generate(float current_time, int i) override;

private:
	Vec3f pos;
	float pos_randomness;
	Vec3f v;
	float v_randomness;

};

class RingGenerator :public Generator {
public:
	
	RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness)
		:pos_randomness(position_randomness), v(velocity), v_randomness(velocity_randomness) {};

	int numNewParticles(float current_time, float dt) const override;

	Particle* Generate(float current_time, int i) override;

	void Paint() const override;

private:
	float pos_randomness;
	Vec3f v;
	float v_randomness;
};