#pragma once

#include "Ray.h"

class Object {
    public:
        Object() {}

        virtual double hit(Ray ray) = 0;
        virtual bool inShadow(Ray ray, vector<shared_ptr<Object>> objects, int idx);
        virtual Pigment getColor() = 0;

};

bool Object::inShadow(Ray ray, vector<shared_ptr<Object>> objects, int idx) {
    double bias = 0.005;
    for (int i = 0; i < objects.size(); i++) {
        if (i != idx && objects[i]->hit(ray) + bias > 0) {
            return true;
        }
    }
    return false;
}