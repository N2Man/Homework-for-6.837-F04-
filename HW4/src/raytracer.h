#pragma once
#include "ray.h"
#include "hit.h"
#include "scene_parser.h"
#include "Object3D.h"
#include "raytree.h"
#include "light.h"

const float epsilon = 0.001;

Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming) {
	Vec3f outcoming;
	Vec3f n = normal;
	Vec3f v = incoming;
	n.Normalize();
	v.Normalize();
	float tmp = v.Dot3(n);
	outcoming = v - 2 * tmp * n;
	return outcoming;
}

bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming, float index_i, float index_t, Vec3f& transmitted) {
	Vec3f n = normal;
	Vec3f i = incoming;
	n.Normalize();
	i.Normalize();
	float tmp = i.Dot3(n);
	Vec3f m = i - tmp * n;
	float sin_i = m.Length();
	float sin_t = sin_i * index_i / index_t;

	if (abs(sin_t) > 1)
		return false;

	float outcos = sqrt(1 - sin_t * sin_t);
	m.Normalize();
	if (tmp < 0)
		transmitted = -outcos * n + sin_t * m;
	else
		transmitted = outcos * n + sin_t * m;

	return true;

}


class RayTracer {
public:
	SceneParser* sp;
	int max_bounces;
	float w;
	bool shadow;
	

	RayTracer() {}
	RayTracer(SceneParser* s, int max_bounces, float cutoff_weight, bool shadows) : 
		sp(s), max_bounces(max_bounces), w(cutoff_weight), shadow(shadows) {}
	
	

	Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit& hit) const {
		vector<Material*> materials;
		vector<Light*> lights;
		for (int i = 0; i < sp->getNumMaterials(); i++)
		{
			materials.push_back(sp->getMaterial(i));
		}

		for (int i = 0; i < sp->getNumLights(); i++)
		{
			lights.push_back(sp->getLight(i));
		}
		if (bounces > max_bounces)
			return Vec3f(0, 0, 0);
		if (weight < weight)
			return Vec3f(0, 0, 0);
		if (sp->getGroup()->intersect(ray, hit, epsilon))
		{
			
			Vec3f pt = hit.getIntersectionPoint();
			Vec3f pt_normal = hit.getNormal();
			pt_normal.Normalize();
			Vec3f color = hit.getMaterial()->getDiffuseColor() * sp->getAmbientLight();
			Vec3f dir2light;
			float dist2light;
			//Shadow
			for (int l = 0; l < sp->getNumLights(); l++)
			{
				Vec3f light_color;
				lights[l]->getIllumination(pt, dir2light, light_color, dist2light);
				if (shadow)
				{
					Ray shadow_ray(pt, dir2light);
					Hit shadow_hit(dist2light, materials[0], Vec3f(0, 0, 0));
					Vec3f tmp;
					if (!sp->getGroup()->intersectShadowRay(shadow_ray, shadow_hit, epsilon))
					{
						tmp = hit.getMaterial()->Shade(ray, hit, dir2light, light_color);
					}
					
					color += tmp;
					RayTree::AddShadowSegment(shadow_ray, 0.0f, shadow_hit.getT());
				}
				else
				{
					color += hit.getMaterial()->Shade(ray, hit, dir2light, light_color);
				}
			}
			//mirror
			if (bounces < max_bounces && hit.getMaterial()->getReflectiveColor().Length() > 0) {
				Ray reflectRay(hit.getIntersectionPoint(), mirrorDirection(hit.getNormal(), ray.getDirection()));
				Hit reflectHit;
				color += traceRay(reflectRay, epsilon, bounces + 1, weight, indexOfRefraction, reflectHit) * hit.getMaterial()->getReflectiveColor();
			}
			//transparent
			if (bounces < max_bounces && hit.getMaterial()->getTransparentColor().Length() > 0) {
				Hit transHit;
				Vec3f transDir;
				bool check;
				if (hit.getNormal().Dot3(ray.getDirection())) {
					check = transmittedDirection(hit.getNormal(), ray.getDirection(), indexOfRefraction, 1.0, transDir);
				}
				else {
					check = transmittedDirection(hit.getNormal(), ray.getDirection(), 1.0, hit.getMaterial()->getIndexOfRefraction(), transDir);
				}

				Ray transRay(hit.getIntersectionPoint(), transDir);
				if (check) {
					if (hit.getNormal().Dot3(ray.getDirection())) {
						color += traceRay(transRay, epsilon, bounces + 1, weight, 1.0, transHit) * hit.getMaterial()->getTransparentColor();
					}
					else {
						color += traceRay(transRay, epsilon, bounces + 1, weight, hit.getMaterial()->getIndexOfRefraction(), transHit) * hit.getMaterial()->getTransparentColor();
					}
				}
			}
			return color;
		}
		return sp->getBackgroundColor();
	}

    /*Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight,
        float indexOfRefraction, Hit& hit) const
    {
		vector<Material*> materials;
		vector<Light*> lights;
		for (int i = 0; i < sp->getNumMaterials(); i++)
		{
			materials.push_back(sp->getMaterial(i));
		}

		for (int i = 0; i < sp->getNumLights(); i++)
		{
			lights.push_back(sp->getLight(i));
		}
        if (bounces > max_bounces)
            return Vec3f(0, 0, 0);
        if (weight < w)
            return Vec3f(0, 0, 0);
        if (sp->getGroup()->intersect(ray, hit, epsilon))
        {
            
            Vec3f pt = hit.getIntersectionPoint();
            Vec3f pt_normal = hit.getNormal();
            pt_normal.Normalize();
            Vec3f color = hit.getMaterial()->getDiffuseColor() * sp->getAmbientLight();
            Vec3f dir2light;
            float dist2light;
            //Shadow
            for (int l = 0; l < sp->getNumLights(); l++)
            {
                Vec3f light_color;
                lights[l]->getIllumination(pt, dir2light, light_color, dist2light);
                if (shadow)
                {
                    Ray shadow_ray(pt, dir2light);
                    Hit shadow_hit(dist2light, materials[0], Vec3f(0, 0, 0));
                    Vec3f tmp;
                    if (sp->getGroup()->intersectShadowRay(shadow_ray, shadow_hit, epsilon))
                    {
                        tmp = hit.getMaterial()->Shade(ray, hit, dir2light, light_color);
                    }
                    
                    color += tmp;
                    RayTree::AddShadowSegment(shadow_ray, 0.0f, shadow_hit.getT());
                }
                else
                {
                    color += hit.getMaterial()->Shade(ray, hit, dir2light, light_color);
                }
            }
            //Mirror
            Vec3f mirror_color = hit.getMaterial()->getReflectiveColor();
            if (mirror_color != Vec3f(0, 0, 0))
            {
                Vec3f mirror_dir = mirrorDirection(pt_normal, ray.getDirection());
                Ray mirror_ray(pt, mirror_dir);
                Hit mirror_hit(dist2light, materials[0], Vec3f(0, 0, 0));
                Vec3f mirror_shade = traceRay(mirror_ray, epsilon, bounces + 1, weight * mirror_color.Length(),
                    indexOfRefraction, mirror_hit);
              
                color += mirror_color * mirror_shade;
                RayTree::AddReflectedSegment(mirror_ray, 0.0f, mirror_hit.getT());
            }
            //Transparent
            Vec3f trans_color = hit.getMaterial()->getTransparentColor();
            if (trans_color != Vec3f(0, 0, 0))
            {
                Vec3f trans_dir;
                float index_t;
                float index_i;
                //Outside
                if (pt_normal.Dot3(ray.getDirection()) < 0)
                {
                    index_i = 1.0f;
                    index_t = hit.getMaterial()->getIndexOfReflection();
                }
                //Inside
                else
                {
                    index_t = 1.0f;
                    index_i = hit.getMaterial()->getIndexOfReflection();
                    pt_normal.Negate();
                }
                //If transmitted
                if (transmittedDirection(pt_normal, ray.getDirection(), index_i, index_t, trans_dir))
                {
                    Ray trans_ray(pt, trans_dir);
                    Hit trans_hit(FLT_MAX, materials[0], Vec3f(0, 0, 0));
                    Vec3f trans_shade = traceRay(trans_ray, epsilon, bounces, weight * trans_color.Length(), index_t, trans_hit);
                    
                    color += trans_color * trans_shade;
                    RayTree::AddTransmittedSegment(trans_ray, 0.0f, trans_hit.getT());
                }
            }
            
            return color;
        }
        return sp->getBackgroundColor();
    }*/
};