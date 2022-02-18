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

class Integrator {
public:
	Integrator() {}
	~Integrator() {}

	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) = 0;
	virtual Vec3f getColor() = 0;
};

class EulerIntegrator :public Integrator {
public:
	EulerIntegrator() {};
	~EulerIntegrator() {};

	void Update(Particle* particle, ForceField* forcefield, float t, float dt) override;
	Vec3f getColor() override {
		return Vec3f(1, 0, 0);
	}
};

class MidPointIntegrator :public Integrator {
public:
	MidPointIntegrator() {};
	~MidPointIntegrator() {};

	void Update(Particle* particle, ForceField* forcefield, float t, float dt) override;
	Vec3f getColor() override {
		return Vec3f(0, 1, 0);
	}
};