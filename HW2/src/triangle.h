#pragma once
#include "Object3D.h"
#include "material.h"

float det2(float a, float b,
    float c, float d) {
    return a * d - b * c;
}

float det3(float a1, float a2, float a3,
    float b1, float b2, float b3,
    float c1, float c2, float c3) {
    return
        a1 * det2(b2, b3, c2, c3)
        - b1 * det2(a2, a3, c2, c3)
        + c1 * det2(a2, a3, b2, b3);
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
};