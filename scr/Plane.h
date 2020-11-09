#pragma once

#include "vec3.h"
#include "Pigment.h"
#include "Object.h"

class Plane : public Object {
    public:
        Plane() {}

        Plane(vec3 normal, double distance) :
            normal(normal), distance(distance) {}
    

        vec3 getNormal() { return normal; }
        double getDistance() { return distance; }
        Pigment getPigment() { return pigment; }

        void setNormal(vec3 normal) { this->normal = normal; }
        void setDistance(double distance) { this->distance = distance; }
        void setPigment(vec3 pigment) { this->pigment.setPigment(pigment); }
        void setFuzzy(double fuzzy) { 
            this->fuzzy = fuzzy; 
            this->type = 1;
        }
        void setIor(double ior) {
            this->ior = ior;
            this->type = 3;
        }

        virtual double hit(Ray ray);
        virtual Pigment getColor() { return pigment; }
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0));
        virtual int getSurfaceType() { return type; }
        virtual double getFuzzy() { return fuzzy; };
        virtual double getIor() { return ior; }

    private:
        vec3 normal;
        double distance;
        Pigment pigment;
        double fuzzy = 0;
        int type = 0;
        double ior = 0;

};

// returns the t value along the ray direction intersecting the plane
double Plane::hit(Ray ray) {
    vec3 n = normal;
    vec3 dir = ray.direction;

    n.normalize();
    double deno = n.dot(dir);
    if (deno != 0.0f)
        return (distance - ray.position.dot(n)) / n.dot(dir);
    
    return -1;
}

vec3 Plane::getN(vec3 curPos) {
    vec3 n = normal;
    n.normalize();
    return n;
}