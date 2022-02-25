#pragma once

#include "scene_parser.h"
#include "object3d.h"
#include "light.h"
#include "rayTree.h"
#include "grid.h"
extern bool shadows;
extern bool shade_back;
extern int nx, ny, nz;

class RayTracer {
public:
    RayTracer(SceneParser* s, int max_bounces, float min_weight) : scene(s), max_bounces(max_bounces), min_weight(min_weight) {
        if (nx != 0) {
            grid = new Grid(s->getGroup()->getBoundingBox(), nx, ny, nz);
            s->getGroup()->insertIntoGrid(grid, nullptr);
        }
        else {
            grid = nullptr;
        }
    }

    ~RayTracer() {}

    Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight, Hit& hit);
    Vec3f RayCast(Ray& ray, float tmin, int bounces, float weight, Hit& hit);
    Vec3f RayCastFast(Ray& ray, float tmin, int bounces, float weight, Hit& hit);

    Grid* getGrid() {
        return grid;
    }

    SceneParser* scene;
    int max_bounces;
    float min_weight;
    Grid* grid;
};


