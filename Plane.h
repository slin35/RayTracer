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
    
        virtual double hit(Ray ray);
        virtual Pigment getColor() { return pigment; }
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0));

    private:
        vec3 normal;
        double distance;
        Pigment pigment;

};

// returns the t value along the ray direction intersecting the plane
double Plane::hit(Ray ray) {
    vec3 n = normal;
    vec3 dir = ray.direction;

    n.normalize();
    dir.normalize();

    return (distance - ray.position.dot(n)) / (dir.dot(n));
}

vec3 Plane::getN(vec3 curPos) {
    return normal;
}