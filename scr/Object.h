#pragma once

#include "Ray.h"
#include "vec3.h"
#include "Pigment.h"

class Object {
    public:
        Object() {}

        virtual double hit(Ray ray) = 0;
        virtual double inShadow(Ray ray, vector<shared_ptr<Object>>* objects, shared_ptr<Object> object);
        virtual Pigment getColor() = 0;
        virtual double getFuzzy() { return 0; }
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0)) = 0;
        virtual int getSurfaceType() { return 0; }
        virtual double getIor() { return 0; }
        virtual void setSurfaceType(int type) {};

};

// check if the given geometry is in shadow
double Object::inShadow(Ray ray, vector<shared_ptr<Object>>* objects, shared_ptr<Object> object) {
    double bias = 0.0001;  // fixing shadow acne
    double res;

    for (auto o : *objects) {
        if (o != object) {
            res = o->hit(ray);
            if (res > bias)
                return res;
        }
    }
    return -1;
}