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

double Object::inShadow(Ray ray, vector<shared_ptr<Object>> objects, int idx) {
    double bias = 0.001;
    double res;

    for (int i = 0; i < objects.size(); i++) {
        if (i != idx) {
            res = objects[i]->hit(ray);
            if (res > bias) {
                return res;
            }
                
        }
    }
    return -1;
}