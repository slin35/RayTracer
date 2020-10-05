#pragma once

#include "vec3.h"
#include "Pigment.h"

class Plane {
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
    
    private:
        vec3 normal;
        double distance;
        Pigment pigment;

};