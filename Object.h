#pragma once

#include "Ray.h"
#include "vec3.h"
#include "Pigment.h"

class Object {
    public:
        Object() {}

        virtual double hit(Ray ray) = 0;
        virtual double inShadow(Ray ray, vector<shared_ptr<Object>> objects, int idx);
        virtual Pigment getColor() = 0;
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0)) = 0;

};

// check if the given geometry is in shadow
double Object::inShadow(Ray ray, vector<shared_ptr<Object>> objects, int idx) {
    double bias = 0.001;  // fixing shadow acne
    double res;

    for (int i = 0; i < objects.size(); i++) {
        // if the object is not itself and hits another object, returns the t value along the ray direction
        if (i != idx) {
            res = objects[i]->hit(ray);
            if (res > bias) {
                return res;
            }
                
        }
    }
    return -1;
}