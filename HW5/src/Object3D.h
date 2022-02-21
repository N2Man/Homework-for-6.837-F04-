#pragma once
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "matrix.h"

class Object3D {
public:
	Material* material;
	
	Object3D() : material(nullptr) {};
	Object3D(Material* m) : material(m) {};
	~Object3D() {
		if (material != nullptr) {
			delete material;
		}
	}
	virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
	virtual void paint() {};
};

class Group : public Object3D {
private:
	int objectsNumber;
	Object3D** objs;
public:
	Group() :objectsNumber(0), objs(nullptr) {};
	Group(int n) :objectsNumber(n) {
		objs = new Object3D * [n];
	}
	~Group() {
		for (int i = 0; i < objectsNumber; i++) {
			delete objs[i];
		}
		delete[] objs;
	}
	void addObject(int index, Object3D* obj) {
		assert(index < objectsNumber);
		objs[index] = obj;
	}

	bool intersect(const Ray& r, Hit& h, float tmin) {
		bool res = false;
		for (int i = 0; i < objectsNumber; i++)
		{
			if (objs[i] == nullptr) continue;
			res = objs[i]->intersect(r, h, tmin) || res;
		}
		return res;
	}

	void paint() {
		for (int i = 0; i < objectsNumber; i++) {
			objs[i]->paint();
		}
	}

	bool intersectShadowRay(const Ray& r, Hit& h, float tmin)
	{
		bool res = false;
		for (int i = 0; i < objectsNumber; i++)
		{
			if (objs[i] == nullptr)
				continue;
			res = objs[i]->intersect(r, h, tmin) || res;
			if (res)
				return true;
		}
		return false;
	}
};

class Plane : public Object3D {
private:
	float distance;
	Vec3f n;
	Material* material;
public:
	Plane() {}
	Plane(Vec3f& normal, float d, Material* m) :material(m), n(normal), distance(d) {};
	bool intersect(const Ray& r, Hit& h, float tmin) {
		Vec3f Rd = r.getDirection();
		Vec3f Ro = r.getOrigin();
		float t = (distance - Ro.Dot3(n)) / Rd.Dot3(n);
		if ((t > max(tmin, 0.0f)) && (t < h.getT()))
		{
			h.set(t, material, n, r);
			return true;
		}
		return false;
	}

	void paint() {
		Vec3f b1, b2, v(1, 0, 0);
		if (n.Dot3(v) == 0) {
			v = Vec3f(0, 1, 0);
		}
		Vec3f::Cross3(b1, v, n);
		b1.Normalize();
		Vec3f::Cross3(b2, n, b1);
		b2.Normalize();
		b1 *= 1000;
		b2 *= 1000;
		Vec3f a = b1 + b2;
		Vec3f b = (-1) * b1 + b2;
		Vec3f c = (-1) * b1 - b2;
		Vec3f d = b2 - b1;
		material->glSetMaterial();
		glBegin(GL_QUADS);
		glNormal3f(n.x(), n.y(), n.z());
		glVertex3f(a.x(), a.y(), a.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(c.x(), c.y(), c.z());
		glVertex3f(d.x(), d.y(), d.z());
		glEnd();
	}
};

extern int theta_steps;
extern int phi_steps;
extern bool gouraud;
const double PI = 3.141592653;


class Sphere : public Object3D {
private:
	Vec3f center;
	double radius;
	//Material* material;
public:
	Sphere() : center(0, 0, 0), radius(0) {};
	Sphere(Vec3f c, double r, Material* m) : Object3D(m), center(c), radius(r) {};
	~Sphere() {};

	bool intersect(const Ray& r, Hit& h, float tmin) {
		Vec3f Rd = r.getDirection();
		Vec3f Ro = r.getOrigin() - center;
		double a = Rd.Length() * Rd.Length();
		double b = 2 * Rd.Dot3(Ro);
		double c = Ro.Dot3(Ro) - radius * radius;
		double d = sqrt(b * b - 4 * a * c);

		if (d < 0) return false;

		double t_pos = (-1 * b + d) / (2 * a);//()!
		double t_neg = (-1 * b - d) / (2 * a);

		if (t_pos < 0) return false;

		double t_now = h.getT();
		if (t_neg > 0) {
			if (t_neg < t_now && t_neg > tmin) {
				Vec3f normal = Ro + t_neg * Rd;
				normal.Normalize();
				h.set(t_neg, material, normal, r);
				return true;
			}
		}
		else {
			if (t_pos < t_now && t_pos > tmin) {
				Vec3f normal = Ro + t_pos * Rd;
				normal.Normalize();
				h.set(t_pos, material, normal, r);
				return true;
			}
		}
		return false;
	}

	void paint() {
		material->glSetMaterial();
		float theta = 2 * PI / theta_steps;
		float phi = PI / phi_steps;

		Vec3f* points = new Vec3f[theta_steps * (phi_steps + 1)];

		// initialize all the points
		Vec3f top = center + Vec3f(0, 1, 0) * radius;
		Vec3f below = center + Vec3f(0, -1, 0) * radius;

		for (int iPhi = 0; iPhi < phi_steps + 1; iPhi++)
		{
			for (int iTheta = 0; iTheta < theta_steps; iTheta++)
			{
				int index = theta_steps * iPhi + iTheta;
				if (iPhi == 0)
				{
					points[index] = below;
					continue;
				}
				if (iPhi == phi_steps)
				{
					points[index] = top;
					continue;
				}
				float tphi = iPhi * phi - PI / 2;
				float ttheta = theta * iTheta;
				points[index] = center + radius * sin(tphi) * Vec3f(0, 1, 0)
					+ radius * cos(tphi) * cos(ttheta) * Vec3f(1, 0, 0) + radius * cos(tphi) * sin(ttheta) * Vec3f(0, 0, 1);
			}
		}

		glBegin(GL_QUADS);
		for (int iPhi = 0; iPhi < phi_steps; iPhi++)
		{
			for (int iTheta = 0; iTheta < theta_steps; iTheta++)
			{
				
				int index[4] = { theta_steps * iPhi + iTheta ,theta_steps * iPhi + (iTheta + 1) % theta_steps,
					theta_steps * (iPhi + 1) + (iTheta + 1) % theta_steps ,theta_steps * (iPhi + 1) + iTheta };
				Vec3f normal;
				if (!gouraud)
				{
					Vec3f::Cross3(normal, points[index[0]] - points[index[1]], points[index[2]] - points[index[1]]);
					if (normal.Length() == 0)
					{
						Vec3f::Cross3(normal, points[index[1]] - points[index[2]], points[index[3]] - points[index[2]]);
					}
				}
				for (int i = 0; i < 4; i++)
				{
					if (gouraud)
					{
						normal = points[index[i]] - center;
					}
					normal.Normalize();
					glNormal3f(normal.x(), normal.y(), normal.z());
					glVertex3f(points[index[i]].x(), points[index[i]].y(), points[index[i]].z());
				}
			}
		}

		glEnd();
	}
};

class Transform : public Object3D {
private:
	Object3D* obj;
	Matrix m;
public:
	Transform() {};
	Transform(Matrix& m, Object3D* o) : m(m), obj(o) {};
	bool intersect(const Ray& r, Hit& h, float tmin) {
		Matrix tmp = m;
		if (tmp.Inverse()) {
			Vec3f Rd = r.getDirection();
			Vec3f Ro = r.getOrigin();
			tmp.Transform(Ro);
			tmp.TransformDirection(Rd);
			Ray new_r(Ro, Rd);
			if (obj->intersect(new_r, h, tmin)) {
				Vec3f new_n = h.getNormal();
				tmp.Transpose();
				tmp.TransformDirection(new_n);
				new_n.Normalize();
				h.set(h.getT(), h.getMaterial(), new_n, r);
				return true;
			}
		}
		return false;
	}

	void paint() {
		glPushMatrix();
		GLfloat* glMatrix = m.glGet();
		glMultMatrixf(glMatrix);
		delete[] glMatrix;
		obj->paint();
		glPopMatrix();
	}
};

static float det2(float a, float b, float c, float d) {
	return a * d - b * c;
}

static float det3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3) {
	return a1 * det2(b2, b3, c2, c3) - b1 * det2(a2, a3, c2, c3) + c1 * det2(a2, a3, b2, b3);
}

class Triangle : public Object3D {
private:
	Vec3f normal;
	Vec3f a;
	Vec3f b;
	Vec3f c;

public:
	Triangle() {};
	Triangle(Vec3f& a, Vec3f& b, Vec3f& c, Material* m) : Object3D(m), a(a), b(b), c(c) {
		Vec3f AB = b - a;
		Vec3f AC = c - a;
		Vec3f::Cross3(normal, AB, AC);
		normal.Normalize();
	}

	bool intersect(const Ray& r, Hit& h, float tmin) {
		Vec3f AB = a - b;
		Vec3f AC = a - c;
		Vec3f Rd = r.getDirection();
		Vec3f Ro = r.getOrigin();
		float A = det3(AB.x(), AC.x(), Rd.x(),
			AB.y(), AC.y(), Rd.y(),
			AB.z(), AC.z(), Rd.z());
		float beta = det3(a.x() - Ro.x(), AC.x(), Rd.x(),
			a.y() - Ro.y(), AC.y(), Rd.y(),
			a.z() - Ro.z(), AC.z(), Rd.z()) / A;
		float gamma = det3(AB.x(), a.x() - Ro.x(), Rd.x(),
			AB.y(), a.y() - Ro.y(), Rd.y(),
			AB.z(), a.z() - Ro.z(), Rd.z()) / A;
		float t = det3(AB.x(), AC.x(), a.x() - Ro.x(),
			AB.y(), AC.y(), a.y() - Ro.y(),
			AB.z(), AC.z(), a.z() - Ro.z()) / A;
		if (t > max(0.1f, tmin) && beta + gamma <= 1 && beta >= 0 && gamma >= 0 && t < h.getT())
		{
			h.set(t, material, normal, r);
			return true;
		}
		return false;
	}

	void paint() {
		material->glSetMaterial();
		glBegin(GL_TRIANGLES);
		glNormal3f(normal.x(), normal.y(), normal.z());
		glVertex3f(a.x(), a.y(), a.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(c.x(), c.y(), c.z());
		glEnd();
	}
};