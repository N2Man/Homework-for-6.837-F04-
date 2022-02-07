#pragma once
#include "Object3D.h"

class Transform : public Object3D {
private:
	Object3D* obj;
	Matrix m;
public:
	Transform() {};
	Transform(Matrix &m, Object3D* o) : m(m), obj(o) {};
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
};