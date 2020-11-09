#pragma once

#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <limits>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Pigment.h"
#include "vec3.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Ray.h"
#include "Util.h"
#include "Plane.h"
#include "Hit.h"

using namespace std;

class Util {
    public:
        Util() {}

        static double randD(double val);
        static double phongDiffuse(double Kd, vec3 normal, vec3 light);
        static double max(double a, double b) { return a > b ? a : b; }
        static Pigment phongMode(vector<shared_ptr<Light>>* lights, vector<shared_ptr<Object>>* objects, shared_ptr<Object> o, vec3 curPos);
        static Pigment foggyMode(Ray ray, vector<shared_ptr<Object>>* objects, Pigment background, int bounces);
        static void gammaEncoder(Pigment& p, double gamma);
        static double detMatrix(glm::vec3 col1, glm::vec3 col2, glm::vec3 col3);

    private:
        static bool inUnitSphere(vec3 pos);
        static vec3 getPointInUnitSphere();
        static double Shlick(double cos_theta, double ior);
};


double Util::detMatrix(glm::vec3 col1, glm::vec3 col2, glm::vec3 col3) {
    return col1.x * (col2.y * col3.z - col3.y * col2.z)
            - col2.x * (col1.y * col3.z - col3.y * col1.z)
            + col3.x * (col1.y * col2.z - col2.y * col1.z);
}

// generating a random float in range 0 - 1 with a shift default to 0
double Util::randD(double val = 0) {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX) + val;
}

double Util::phongDiffuse(double Kd, vec3 normal, vec3 light) {
    normal.normalize();
    light.normalize();

    return max(0, normal.dot(light)) * Kd;
}

// returning phong diffuse color with shadow feeler for any given geometry
Pigment Util::phongMode(vector<shared_ptr<Light>>* lights, vector<shared_ptr<Object>>* objects, shared_ptr<Object> o, vec3 curPos) {
    Pigment color, shadow;
    vec3 l, n;
    Ray shadowFeeler;
    double res;

    for (auto light : *lights) {
        l = light->getPosition() - curPos;
        n = o->getN(curPos);
        shadowFeeler = Ray(curPos, l);
        res = o->inShadow(shadowFeeler, objects, o);

        if (res > 0) {
            color = color + Pigment(0, 0, 0.02);
        }
        else {
            color = color + o->getColor() * light->getPigment() * Util::phongDiffuse(1, n, l);
        }

    }

    return color;
}

// returning foggy diffuse for any given object
Pigment Util::foggyMode(Ray ray, vector<shared_ptr<Object>>* objects, Pigment background, int bounces) {
    Pigment color;
    double res;
    double offset = 0;
    Hit hit(objects, ray);
    shared_ptr<Object> obj;

    if (bounces == 0) {
        return color;
    }

    res = hit.getClosestHit(obj);

    if (hit.isHit()) {
        vec3 position = ray.getCurrentPos(res);
        vec3 normal = obj->getN(position);
        normal.normalize();

        vec3 s = getPointInUnitSphere();

        Ray scatterRay;

        if (obj->getSurfaceType() == 0) {    // diffuse
            vec3 direction = normal + s + offset;
            scatterRay = Ray(position, direction);
        }
        else if (obj->getSurfaceType() == 1) {   // reflection
            if (obj->getFuzzy() >= 1.0f)
                scatterRay = Ray(position, normal + s);
            else {
                vec3 direction = ray.direction;
                direction.normalize();

                vec3 R = direction - normal * direction.dot(normal) * 2;

                scatterRay = Ray(position, R + s * obj->getFuzzy());
            
            }
        }
        else if (obj->getSurfaceType() == 2) {    // emissive
            return obj->getColor();
        }
        else if (obj->getSurfaceType() == 3) {  // refraction
            vec3 direction = ray.direction;
            direction.normalize();

            double refractRatio = 1.0 / obj->getIor();

            if (direction.dot(normal) > 0) {    // ray's going out of medium
                normal = normal * -1;
                refractRatio = obj->getIor();
            }
            
            double cos_theta = (direction * -1).dot(normal);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
            
            double reflect = Shlick(cos_theta, obj->getIor());

            if (refractRatio * sin_theta > 1.0) {       // total internal reflection && Fresnel
                vec3 R = direction - normal * direction.dot(normal) * 2;
                scatterRay = Ray(position, R);
            }
            else {
                vec3 r_perp = (direction + normal * cos_theta) * refractRatio;
                vec3 r_par = normal * (-1.0 * sqrt(fabs(1.0 - pow(r_perp.leng(), 2))));
                vec3 refraction = r_perp + r_par;

                scatterRay = Ray(position, refraction);
            }
            
        }

        return obj->getColor() * foggyMode(scatterRay, objects, background, bounces - 1);
    }
    return background;
}

// check if the given vector is inside the unit sphere center at (0, 0, 0) or not
bool Util::inUnitSphere(vec3 pos) {
    if (pos.leng() <= 1)
        return true;
    return false;
}

// encode the color with a gamma value
void Util::gammaEncoder(Pigment& p, double gamma = 2.2) {
    p = p^(1/gamma);
}

vec3 Util::getPointInUnitSphere() {
    vec3 s;

    do {
        s = vec3(Util::randD(-0.5) * 2, Util::randD(-0.5) * 2, Util::randD(-0.5) * 2);
    }  while (!inUnitSphere(s));
    return s;
}

double Util::Shlick(double cos_theta, double ior) {
    double r0 = (1.0 - ior) / (1.0 + ior);
    r0 *= r0;
    return r0 + (1.0 - r0) * pow(1 - cos_theta, 5);
}