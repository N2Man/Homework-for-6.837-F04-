#pragma once
#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "ray.h"
#include "hit.h"
#include "vectors.h"
#include "windows.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "glCanvas.h"
#include <vector>

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:
	Material() {};
	// CONSTRUCTORS & DESTRUCTOR
	Material(const Vec3f& d_color) { diffuseColor = d_color; }
	virtual ~Material() {}

	// ACCESSORS
	virtual Vec3f getDiffuseColor() const { return diffuseColor; }

	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const = 0;

	virtual void glSetMaterial(void) const = 0;

    virtual Vec3f getSpecularColor(void) const = 0;
    virtual Vec3f getReflectiveColor(void) const = 0;
    virtual Vec3f getTransparentColor(void) const = 0;
    virtual float getIndexOfRefraction(void) const = 0;
    virtual bool reflect(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const = 0;
    virtual bool refract(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const = 0;

protected:

	// REPRESENTATION
	Vec3f diffuseColor;

};

// ====================================================================
// ====================================================================

#endif


extern bool shade_back;
#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif


class PhongMaterial : public Material {
private:
    Vec3f specularColor;
    float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;

public:
    PhongMaterial() {}
    PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent,
                    const Vec3f& reflectiveColor, const Vec3f& transparentColor, float indexOfRefraction):
                    Material(diffuseColor), specularColor(specularColor), exponent(exponent),
                    reflectiveColor(reflectiveColor), transparentColor(transparentColor), indexOfRefraction(indexOfRefraction) {};

    Vec3f getSpecularColor() const {
        return specularColor;
    }

    Vec3f getReflectiveColor() const {
        return reflectiveColor;
    }

    Vec3f getTransparentColor() const {
        return transparentColor;
    }
    
    float getIndexOfRefraction() const {
        return indexOfRefraction;
    }

    bool reflect(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const {
        if (reflectiveColor.Length() < 0.0001)
            return false;
        Vec3f ray_in = ray.getDirection();
        Vec3f normal = hit.getNormal();
        //if(ray_in.Dot3(normal) > 0)
        //    normal = -1 * normal;
        Vec3f ray_out = ray_in - 2.0f * normal.Dot3(ray_in) * normal;
        ray_out.Normalize();
        reflected = Ray(hit.getIntersectionPoint(), ray_out);
        attenuation = reflectiveColor;
        return true;
    }


    bool refract(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& refracted) const {
        if (transparentColor.Length() < 0.0001)
            return false;
        Vec3f ray_in = ray.getDirection();
        Vec3f normal = hit.getNormal();
        float ni_over_nt;
        if (ray_in.Dot3(normal) > 0) {
            normal = -1 * normal;
            ni_over_nt = indexOfRefraction;
        }
        else {
            ni_over_nt = 1.0f / indexOfRefraction;
        }
        Vec3f v = ray_in * -1;
        float NoV = normal.Dot3(v);
        float t = 1 - ni_over_nt * ni_over_nt * (1 - NoV * NoV);
        if (t > 0) {
            Vec3f ray_out = (ni_over_nt * NoV - sqrt(t)) * normal - ni_over_nt * v;
            ray_out.Normalize();
            refracted = Ray(hit.getIntersectionPoint(), ray_out);
            attenuation = transparentColor;
            return true;
        }
        else {
            return false;
        }
    }


    Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const {
        Vec3f normal = hit.getNormal();
        
        Vec3f dirToCamera = ray.getDirection();
        
        float clamping = max(normal.Dot3(dirToLight), 0.0f);
        if (shade_back && normal.Dot3(dirToCamera) > 0)
        {
            normal.Negate();
        }
        float distToLight = 1.0f;
        dirToCamera.Negate();
        Vec3f half = dirToCamera + dirToLight;
        half.Normalize();
        float diffuse = max(dirToLight.Dot3(normal), 0.0f);
        float specular = max(normal.Dot3(half), 0.0f);
        float shiness = pow(specular, exponent);
        Vec3f color = diffuseColor * diffuse + specularColor * shiness;
        color = color * lightColor * (1 / pow(distToLight, 2));
        color = color * clamping;
        return color;
    }

    void glSetMaterial(void) const {
        GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
        GLfloat specular[4] = {
          getSpecularColor().r(),
          getSpecularColor().g(),
          getSpecularColor().b(),
          1.0 };
        GLfloat diffuse[4] = {
          getDiffuseColor().r(),
          getDiffuseColor().g(),
          getDiffuseColor().b(),
          1.0 };

        // NOTE: GL uses the Blinn Torrance version of Phong...      
        float glexponent = exponent;
        if (glexponent < 0) glexponent = 0;
        if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX 

        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

        // OPTIONAL: 3 pass rendering to fix the specular highlight 
        // artifact for small specular exponents (wide specular lobe)

        if (SPECULAR_FIX_WHICH_PASS == 0) {
            // First pass, draw only the specular highlights
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

        }
        else if (SPECULAR_FIX_WHICH_PASS == 1) {
            // Second pass, compute normal dot light 
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
        }
        else {
            // Third pass, add ambient & diffuse terms
            assert(SPECULAR_FIX_WHICH_PASS == 2);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
        }

#endif
    }
};