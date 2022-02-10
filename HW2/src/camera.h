#pragma once
#include "ray.h"
#include "matrix.h"

class Camera {
protected:
    float ratio = 1.0f;//width/height
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};
