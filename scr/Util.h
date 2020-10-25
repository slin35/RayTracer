#pragma once

#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <limits.h>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>

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
        static Pigment phongMode(vector<shared_ptr<Light>>* lights, vector<shared_ptr<Object>>* objects, vector<shared_ptr<Object>>* objects2, shared_ptr<Object> o, vec3 curPos);
        static Pigment foggyMode(Ray ray, vector<shared_ptr<Object>>* objects, Pigment background, int bounces);
        static void gammaEncoder(Pigment& p, double gamma);
        static double detMatrix(glm::vec3 col1, glm::vec3 col2, glm::vec3 col3);

    private:
        static bool inUnitSphere(vec3 pos);
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
Pigment Util::phongMode(vector<shared_ptr<Light>>* lights, vector<shared_ptr<Object>>* objects, vector<shared_ptr<Object>>* objects2, shared_ptr<Object> o, vec3 curPos) {
    Pigment color, shadow;
    vec3 l, n;
    Ray shadowFeeler;
    double res;

    for (auto light : *lights) {
        l = light->getPosition() - curPos;
        n = o->getN(curPos);
        shadowFeeler = Ray(curPos, l);
        res = o->inShadow(shadowFeeler, objects2, o);

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
    double offset = 0.001;
    Hit hit(objects, ray);
    shared_ptr<Object> obj;

    if (bounces == 0) {
        return color;
    }

    res = hit.getClosestHit(obj);

    if (hit.isHit()) {
        vec3 s = vec3(Util::randD(-0.5) * 2, Util::randD(-0.5) * 2, Util::randD(-0.5) * 2);
        while (!inUnitSphere(s))
            s = vec3(Util::randD(-0.5) * 2, Util::randD(-0.5) * 2, Util::randD(-0.5) * 2);
        vec3 position = ray.getCurrentPos(res);
        vec3 normal = obj->getN(position);

        normal.normalize();

        vec3 direction = normal + s + offset;

        Ray scatterRay = Ray(position, direction);

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