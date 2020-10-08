#pragma once

#include <stdlib.h>

#include "Pigment.h"
#include "vec3.h"

class Util {
    public:
        Util() {}

        static double randD(double val);
        static double phongDiffuse(double Kd, vec3 normal, vec3 light);
        static double max(double a, double b) { return a > b ? a : b; }
};

double Util::randD(double val) {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX) + val;
}

double Util::phongDiffuse(double Kd, vec3 normal, vec3 light) {
    normal.normalize();
    light.normalize();

    return max(0, normal.dot(light)) * Kd;
}

