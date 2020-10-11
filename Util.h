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
        static Pigment phongMode(vector<shared_ptr<Light>> lights, vector<shared_ptr<Object>> objects, shared_ptr<Object> o, vec3 curPos);
        static Pigment foggyMode(vector<shared_ptr<Light>> lights, shared_ptr<Object> object, Ray ray, vector<shared_ptr<Object>> objects, Pigment background, int bounces);
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

Pigment Util::phongMode(vector<shared_ptr<Light>> lights, vector<shared_ptr<Object>> objects, shared_ptr<Object> o, vec3 curPos) {
    Pigment color, shadow;
    vec3 l, n;
    Ray shadowFeeler;
    auto it = find(objects.begin(), objects.end(), o);
    double res;

    for (auto light : lights) {
        l = light->getPosition() - curPos;
        n = o->getN(curPos);
        shadowFeeler = Ray(curPos, l);
        res = o->inShadow(shadowFeeler, objects, distance(objects.begin(), it));

        if (res > 0) {
            color = color + Pigment(0, 0, 0.02);
        }
        else {
            color = color + o->getColor() * light->getPigment() * Util::phongDiffuse(1, n, l);
        }

    }

    return color;
}

Pigment Util::foggyMode(vector<shared_ptr<Light>> lights, shared_ptr<Object> object, Ray ray, vector<shared_ptr<Object>> objects, Pigment background, int bounces) {
    Pigment color;
    double res;
    

    if (bounces == 0) {
        return color;
    }

    for (auto o : objects) {
        res = o->hit(ray);
        if (res > 0) {
            vec3 s = vec3(Util::randD(-0.5) * 2, Util::randD(-0.5) * 2, Util::randD(-0.5) * 2);
            while (!inUnitSphere(s))
                s = vec3(Util::randD(-0.5) * 2, Util::randD(-0.5) * 2, Util::randD(-0.5) * 2);
            vec3 position = ray.getCurrentPos(res);
            vec3 normal = o->getN(position);

            normal.normalize();

            vec3 direction = normal + s;
        //    ray.direction.normalize();

        //    vec3 direction = ray.direction + s;

            Ray scatterRay = Ray(position, direction);

            return o->getColor() * foggyMode(lights, object, scatterRay, objects, background, bounces - 1);
        }
    }

    return background;
}

bool Util::inUnitSphere(vec3 pos) {
    if (pos.leng() <= 1)
        return true;
    return false;
}

void Util::gammaEncoder(Pigment& p, double gamma = 2.2) {
    p = p^(1/gamma);
}