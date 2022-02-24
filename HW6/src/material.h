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

    /*virtual Vec3f getSpecularColor(void) const = 0;
    virtual Vec3f getReflectiveColor(void) const = 0;
    virtual Vec3f getTransparentColor(void) const = 0;
    virtual float getIndexOfRefraction(void) const = 0;*/
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

    PhongMaterial(const Vec3f& d_color) : Material(d_color), exponent(0), indexOfRefraction(0) {}

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

    bool reflect(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const;

    bool refract(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& refracted) const;
    
    Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const override;
    
    void glSetMaterial(void) const override;
    
};

class CheckerBoard : public Material{

public:
    CheckerBoard() {}

    CheckerBoard(Matrix* m, Material* mat1, Material* mat2) : matrix(m),m1(mat1),m2(mat2) {}

    Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& l, const Vec3f& lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const override;

    bool refract(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& refracted) const override;

private:

    Matrix* matrix;
    Material* m1;
    Material* m2;

};


class Noise : public Material {
    
public:
    Noise() {}

    Noise(Matrix* m, Material* mat1, Material* mat2, int octaves) :matrix(m),m1(mat1),m2(mat2),oct(octaves) {}

    Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& l, const Vec3f& lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const override;

    bool refract(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& refracted) const override;

private:
    Matrix* matrix;
    Material* m1;
    Material* m2;
    int oct;
};

class Marble : public Material {

public:
    Marble() {}

    Marble(Matrix* m, Material* mat1, Material* mat2, int octaves, float frequency, float amplitude) 
        : matrix(m), m1(mat1), m2(mat2), oct(octaves), frq(frequency), amp(amplitude) {}

    Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& l, const Vec3f& lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const override;

    bool refract(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& refracted) const override;

    Vec3f getDiffuseColor() const override;

private:
    Matrix* matrix;
    Material* m1;
    Material* m2;
    int oct;
    float frq;
    float amp;

};

class Wood : public Material {
public:
    Wood(Matrix* m, Material* mat1, Material* mat2, int octaves, float frequency, float amplitude) :
        Material(Vec3f()), matrix(m), material1(mat1), material2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude) {}

    Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& l, const Vec3f& lightColor) const override;

    void glSetMaterial() const override;

    bool reflect(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& reflected) const override;

    bool refract(const Ray& ray, const Hit& hit, Vec3f& attenuation, Ray& refracted) const override;

    Vec3f getDiffuseColor() const override;

private:
    Material* material1;
    Material* material2;
    Matrix* matrix;
    int octaves;
    float frequency;
    float amplitude;
};