#pragma once
#include "glCanvas.h"
#include "matrix.h"
#include "parser.h"
#include "particle.h"
#include "particleSet.h"
#include "random.h"
#include "system.h"
#include "vectors.h"

class ForceField {
public:
	ForceField() {}
	~ForceField() {}

	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const = 0;

};

class GravityForceField : public ForceField {
private:
	Vec3f gravity;

public:
	GravityForceField(Vec3f g) : gravity(g) {}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

};

class ConstantForceField : public ForceField {
private:
	Vec3f force;

public:
	ConstantForceField(Vec3f f) : force(f) {}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

};

class RadialForceField : public ForceField {
private:
	float magnitude;

public:
	RadialForceField(float m) : magnitude(m) {}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

};

class VerticalForceField : public ForceField {
private:
	float magnitude;

public:
	VerticalForceField(float m) : magnitude(m) {}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

};

class WindForceField : public ForceField {
public:
	WindForceField(float magnitude) : magnitude(magnitude) {}

	Vec3f getAcceleration(const Vec3f& position, float mass, float t) const override;

private:
	float magnitude;
};