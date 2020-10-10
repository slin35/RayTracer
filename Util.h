#pragma once

#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <limits.h>
#include <math.h>

#include "Pigment.h"
#include "vec3.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Ray.h"
#include "Util.h"
#include "Plane.h"

using namespace std;

class Util {
    public:
        Util() {}

        static double randD(double val);
        static double phongDiffuse(double Kd, vec3 normal, vec3 light);
        static double max(double a, double b) { return a > b ? a : b; }
        static Pigment phongMode(vector<shared_ptr<Light>> lights, vector<shared_ptr<Object>> objects, shared_ptr<Sphere> sphere, vec3 curPos);
        static Pigment phongMode(vector<shared_ptr<Light>> lights, vector<shared_ptr<Object>> objects, shared_ptr<Plane> plane, vec3 curPos);
        static Pigment foggyMode(shared_ptr<Object> object, Ray ray, vector<shared_ptr<Object>> objects, int bounces);
        static void gammaEncoder(Pigment& p, double gamma);

    private:
        static bool inUnitSphere(vec3 pos);
};

double Util::randD(double val = 0) {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX) + val;
}

double Util::phongDiffuse(double Kd, vec3 normal, vec3 light) {
    normal.normalize();
    light.normalize();

    return max(0, normal.dot(light)) * Kd;
}


Pigment Util::phongMode(vector<shared_ptr<Light>> lights, vector<shared_ptr<Object>> objects, shared_ptr<Sphere> sphere, vec3 curPos) {
    Pigment color;
    vec3 normal, l;
    Ray shadowFeeler;

    for (auto light : lights) {
        normal = curPos - sphere->getCenter();
        l = light->getPosition() - curPos;
        auto it = find(objects.begin(), objects.end(), sphere);

        shadowFeeler = Ray(curPos, l);
        if (sphere->inShadow(shadowFeeler, objects, distance(objects.begin(), it)))
            color = color + Pigment(0, 0, 0.01);
        else
            color = color + sphere->getpigment() * lights[0]->getPigment() * Util::phongDiffuse(1, normal, l);

    }

    return color;
}

Pigment Util::phongMode(vector<shared_ptr<Light>> lights, vector<shared_ptr<Object>> objects, shared_ptr<Plane> plane, vec3 curPos) {
    Pigment color;
    vec3 l;
    Ray shadowFeeler;

    for (auto light : lights) {
        l = light->getPosition() - curPos;
        shadowFeeler = Ray(curPos, l);
        auto it = find(objects.begin(), objects.end(), plane);
        if (plane->inShadow(shadowFeeler, objects, distance(objects.begin(), it))) {
            color = color + Pigment(0, 0, 0.01);
        }
        else {
            color = plane->getPigment();
        }
    }

    return color;
}

Pigment Util::foggyMode(shared_ptr<Object> object, Ray ray, vector<shared_ptr<Object>> objects, int bounces) {
    double min = INT_MAX;
    double res;
    Pigment color(1, 1, 1);
    vec3 position = ray.position;

    if (bounces == 0) {
        return object->getColor();
    }

    for (auto obj : objects) {
        if (object != obj) {
            res = obj->hit(ray);
            if (res > 0 && res < min) {
                min = res;
                color = obj->getColor();
                position = ray.getCurrentPos(res);
            }
        }
    }
    vec3 pos = vec3(randD() * 2 - 1, randD() * 2 - 1, randD() * 2 - 1);
    while (!inUnitSphere(pos))
        pos = vec3(randD() * 2 - 1, randD() * 2 - 1, randD() * 2 - 1);
    ray.direction.normalize();
    vec3 n = ray.direction + pos;
    n.normalize();
    Ray scatter = Ray(position, ray.direction + pos);
    return color * foggyMode(object, scatter, objects, bounces - 1);
}

bool Util::inUnitSphere(vec3 pos) {
    if (pos.leng() <= 1)
        return true;
    return false;
}

void Util::gammaEncoder(Pigment& p, double gamma = 2.2) {
    p = p^(1/2.2);
}