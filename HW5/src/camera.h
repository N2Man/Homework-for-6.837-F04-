#pragma once
#include "ray.h"
#include "matrix.h"
#include "windows.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "glCanvas.h"

class Camera {
protected:
    float ratio = 1.0f;//width/height
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0; 
};

class OrthographicCamera :public Camera {
private:
	Vec3f center;
	Vec3f up;
	Vec3f hori;
	Vec3f dir;
	float size;
public:
	OrthographicCamera() {};
	OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s) {
		center = c;
		dir = d;
		up = u;
		dir.Normalize();
		Vec3f::Cross3(hori, dir, up);
		up.Normalize();
		Vec3f::Cross3(up, hori, dir);
		size = s;

	}
	~OrthographicCamera() {};

	Ray generateRay(Vec2f point) {
		Vec3f dx = size * hori;
		Vec3f dy = size * up;
		Vec3f pointToImage = center - 0.5 * dx - 0.5 * dy + point.x() * dx + point.y() * dy;
		return Ray(pointToImage, dir);
	}
	float getTMin() const {
		return -1 * FLT_MAX;
	}

	void glInit(int w, int h) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w > h) glOrtho(-size / 2.0, size / 2.0, -size * (float)h / (float)w / 2.0, size * (float)h / (float)w / 2.0, 0.5, 40.0);
		else glOrtho(-size * (float)w / (float)h / 2.0, size * (float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 40.0);
	}

	void glPlaceCamera(void) {
		gluLookAt(center.x(), center.y(), center.z(), center.x() + dir.x(), center.y() + dir.y(), center.z() + dir.z(), up.x(), up.y(), up.z());
	}

	void dollyCamera(float dist) {
		center += dir * dist;
	}

	void truckCamera(float dx, float dy) {
		Vec3f horizontal;
		Vec3f::Cross3(horizontal, dir, up);
		horizontal.Normalize();
		Vec3f screenUp;
		Vec3f::Cross3(screenUp, horizontal, dir);
		center += horizontal * dx + screenUp * dy;
	}

	void rotateCamera(float rx, float ry) {
		Vec3f horizontal;
		Vec3f::Cross3(horizontal, dir, up);
		horizontal.Normalize();
		// Don't let the model flip upside-down (There is a singularity // at the poles when 'up' and 'direction' are aligned) 
		float tiltAngle = acos(up.Dot3(dir));
		if (tiltAngle - ry > 3.13)
			ry = tiltAngle - 3.13;
		else if (tiltAngle - ry < 0.01)
			ry = tiltAngle - 0.01;
		Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
		rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
		rotMat.Transform(center);
		rotMat.TransformDirection(dir);
	}


};

class PerspectiveCamera : public Camera {
private:
	Vec3f center;
	Vec3f up;
	Vec3f hori;
	Vec3f dir;
	float angle;
public:
	PerspectiveCamera() {}
	PerspectiveCamera(Vec3f& c, Vec3f& d, Vec3f& u, float a) {
		center = c;
		dir = d;
		dir.Normalize();
		Vec3f::Cross3(hori, dir, u);
		hori.Normalize();
		Vec3f::Cross3(up, hori, dir);
		angle = a;
	}
	~PerspectiveCamera() {}

	Ray generateRay(Vec2f point) {
		const float dist = 1.0;
		Vec3f dy = 2 * tan(angle / 2.0) * dist * up;
		Vec3f dx = 2 * tan(angle / 2.0) * dist * ratio * hori;
		Vec3f pt = center + dist * dir - 0.5 * dx + point.x() * dx - 0.5 * dy + point.y() * dy;
		Vec3f dir = pt - center;
		dir.Normalize();
		return Ray(center, dir);
	}
	float getTMin() const {
		return -1 * FLT_MAX;
	}

	void glInit(int w, int h) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(angle * 180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
	}

	void glPlaceCamera(void) {
		gluLookAt(center.x(), center.y(), center.z(), center.x() + dir.x(), center.y() + dir.y(), center.z() + dir.z(), up.x(), up.y(), up.z());
	}

	void dollyCamera(float dist) {
		center += dir * dist;
	}

	void truckCamera(float dx, float dy) {
		Vec3f horizontal;
		Vec3f::Cross3(horizontal, dir, up);
		horizontal.Normalize();
		Vec3f screenUp;
		Vec3f::Cross3(screenUp, horizontal, dir);
		center += horizontal * dx + screenUp * dy;
	}

	void rotateCamera(float rx, float ry) {
		Vec3f horizontal;
		Vec3f::Cross3(horizontal, dir, up);
		horizontal.Normalize();
		// Don't let the model flip upside-down (There is a singularity 
		// at the poles when 'up' and 'direction' are aligned) 
		float tiltAngle = acos(up.Dot3(dir));
		if (tiltAngle - ry > 3.13)
			ry = tiltAngle - 3.13;
		else if (tiltAngle - ry < 0.01)
			ry = tiltAngle - 0.01;
		Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
		rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
		rotMat.Transform(center);
		rotMat.TransformDirection(dir);
		dir.Normalize();
	}
};
