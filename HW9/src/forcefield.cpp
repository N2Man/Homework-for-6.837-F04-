#include "forcefield.h"

Vec3f GravityForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	return gravity;
}

Vec3f ConstantForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	return force * (1.0 / mass);
}

Vec3f RadialForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	return magnitude * (-1 * position) * (1.0f / mass);
}

Vec3f VerticalForceField::getAcceleration(const Vec3f& position, float mass, float t) const
{
	return magnitude * (-1 * position) * (1.0f / mass);
}
