#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vectors.h"
#include "object3d.h"
#include <GL/gl.h>

// ====================================================================
// ====================================================================

class Light {

public:

    // CONSTRUCTOR & DESTRUCTOR
    Light() {}
    virtual ~Light() {}

    // VIRTUAL METHODS
    virtual void getIllumination(const Vec3f& p, Vec3f& dir, Vec3f& col, float& distanceToLight) const = 0;
    virtual void glInit(int id) = 0;
};

// ====================================================================
// ====================================================================

class DirectionalLight : public Light {

public:

    // CONSTRUCTOR & DESTRUCTOR
    DirectionalLight() {
        direction = Vec3f(0, 0, 0);
        color = Vec3f(1, 1, 1);
    }
    DirectionalLight(const Vec3f& d, const Vec3f& c) {
        direction = d; direction.Normalize();
        color = c;
    }
    ~DirectionalLight() {}

    // VIRTUAL METHODS
    void getIllumination(const Vec3f& p, Vec3f& dir, Vec3f& col,
        float& distanceToLight) const {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = direction * (-1.0f);
        col = color;
        // the distance is INFINITY
        distanceToLight = INFINITY;
    }

    void glInit(int id);

private:

    // REPRESENTATION
    Vec3f direction;
    Vec3f color;

};

void DirectionalLight::glInit(int id) {
    GLenum glLightID;
    switch (id) {
    case 0: glLightID = GL_LIGHT0; break;
    case 1: glLightID = GL_LIGHT1; break;
    case 2: glLightID = GL_LIGHT2; break;
    case 3: glLightID = GL_LIGHT3; break;
    case 4: glLightID = GL_LIGHT4; break;
    case 5: glLightID = GL_LIGHT5; break;
    case 6: glLightID = GL_LIGHT6; break;
    case 7: glLightID = GL_LIGHT7; break;
    default: glLightID = GL_LIGHT7;
    }

    // Set the last component of the position to 0 to indicate
    // a directional light source
    GLfloat glPosition[4];
    glPosition[0] = -direction.x();
    glPosition[1] = -direction.y();
    glPosition[2] = -direction.z();
    glPosition[3] = 0.0;

    GLfloat glColor[4];
    glColor[0] = color.r();
    glColor[1] = color.g();
    glColor[2] = color.b();
    glColor[3] = 1.0;

    glLightfv(glLightID, GL_POSITION, glPosition);
    glLightfv(glLightID, GL_DIFFUSE, glColor);
    glLightfv(glLightID, GL_SPECULAR, glColor);
    glEnable(glLightID);
}

// ====================================================================
// ====================================================================

#endif