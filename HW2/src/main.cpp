#include "camera.h"
#include "group.h"
#include "hit.h"
#include "image.h"
#include "material.h"
#include "matrix.h"
#include "Object3D.h"
#include "OrthographicCamera.h"
#include "ray.h"
#include "scene_parser.h"
#include "sphere.h"
#include "vectors.h"
#include "light.h"
#include <vector>


int main(int argc, char** argv) {
// ========================================================
// ========================================================
// Some sample code you might like to use for parsing 
// command line arguments 

    char* input_file = NULL;
    int width = 100;
    int height = 100;
    char* output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char* depth_file = NULL;
    char* normal_file = NULL;
    bool shade_back = false;

    // sample command line:
    // raytracer -input scene1_01.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++; assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-size")) {
            i++; assert(i < argc);
            width = atoi(argv[i]);
            i++; assert(i < argc);
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output")) {
            i++; assert(i < argc);
            output_file = argv[i];
        }
        else if (!strcmp(argv[i], "-depth")) {
            i++; assert(i < argc);
            depth_min = atof(argv[i]);
            i++; assert(i < argc);
            depth_max = atof(argv[i]);
            i++; assert(i < argc);
            depth_file = argv[i];
        }
        else if (!strcmp(argv[i], "-normals")) {
            i++; assert(i < argc);
            normal_file = argv[i];
        }
        else if (!strcmp(argv[i], "-shade_back")) {
            i++; assert(i < argc);
            shade_back = true;
        }
        else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    // ========================================================
    // ========================================================
    // read file by scene_parser
    SceneParser sp(input_file);
    Camera* camera = sp.getCamera();
    Vec3f BackgroundColor = sp.getBackgroundColor();
    int n_materials = sp.getNumMaterials();
    vector<Material*> materials;
    for (int i = 0; i < n_materials; i++) {
        materials.push_back(sp.getMaterial(i));
    }
    int n_lights = sp.getNumLights();
    vector<Light*> lights;
    for (int i = 0; i < n_lights; i++) {
        lights.push_back(sp.getLight(i));
    }
    Group* group = sp.getGroup();
    Vec3f ambient = sp.getAmbientLight();

    //create the image
    Image img(width, height);
    img.SetAllPixels(BackgroundColor);
    Image depth_img(width, height);
    
    // ray tracing
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float dx = x / (float)width;
            float dy = y / (float)height;
            Ray r = camera->generateRay(Vec2f(dx, dy));
            Hit h = Hit(FLT_MAX, materials[0], Vec3f(0, 0, 0));
            if (group->intersect(r, h, camera->getTMin())) {
                // diffuse
                Vec3f color(0, 0, 0);
                Vec3f p = h.getIntersectionPoint();
                Vec3f p_normal = h.getNormal();
                if (shade_back && p_normal.Dot3(r.getDirection()) > 0) {
                    p_normal = -1 * p_normal;
                }
                Vec3f dif = h.getMaterial()->getDiffuseColor();
                color += ambient;

                //light
                Vec3f light_dir;
                for (int i = 0; i < n_lights; i++) {
                    Vec3f light_color;
                    lights[i]->getIllumination(p, light_dir, light_color);
                    color += light_color * max(p_normal.Dot3(light_color), 0.0f);
                }

                color.Set(color.x() * dif.x(), color.y() * dif.y(), color.z() * dif.z());

                img.SetPixel(x, y, color);

                //depth_map
                float depth = h.getT();
                depth = max(depth_min, depth);
                depth = min(depth_max, depth);
                float gray = 1 - (depth - depth_min) / (depth_max - depth_min);
                depth_img.SetPixel(x, y, Vec3f(gray, gray, gray));
            }
        }
    }

    img.SaveTGA(output_file);
    depth_img.SaveTGA(depth_file);
    return 0;
}