#pragma once

#include <vector>
#include <iostream> 
#include <algorithm>


#include "Pigment.h"
#include "vec3.h"
#include "Object.h"

using namespace std;

class Sphere : public Object {
    public:
        Sphere() {}

        Sphere(vec3& center, float radius, Pigment& pigment) :
            center(center), radius(radius), pigment{pigment} {}


        void setCenter(vec3 center) { this->center = center; }
        void setRadius(double raidus) { this->radius = raidus; }
        void setPigment(vec3 pigment) { this->pigment.setPigment(pigment); }
        void setFuzzy(double fuzzy) { 
            this->fuzzy = fuzzy; 
            this->type = 1;
        }
        void setIor(double ior) {
            this->ior = ior;
            this->type = 3;
        }

        vec3 getCenter() const { return center; }
        double getRadius() const { return radius; }
        Pigment getpigment() const { return pigment; }

        virtual double getFuzzy() { return fuzzy; }
        virtual double getIor() { return ior; }

        virtual double hit(Ray ray);
        virtual Pigment getColor() { return pigment; }
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0));
        virtual int getSurfaceType() { return type; }

    private:
        vec3 center;
        double radius = 0;
        Pigment pigment{1.0f, 1.0f, 1.0f};
        double fuzzy = 0;
        int type = 0;
        double ior;

};

// returns the t value along the ray direction intersecting the sphere
double Sphere::hit(Ray ray) {
    vec3 e = ray.position;
    vec3 d = ray.direction;
    double r = radius;

    double a = d.dot(d), b = 2 * d.dot(e - center), c = (e - center).dot(e - center) - r * r;
    
    double discriminant = b * b - 4 * a * c;

    
    if (discriminant < 0)
        return -1;

    double t1 = max((-b + sqrt(discriminant))/2/a, 0.0);
    double t2 = max((-b - sqrt(discriminant))/2/a, 0.0);

    return min(t1, t2);
}

vec3 Sphere::getN(vec3 curPos) {
    vec3 normal = curPos - center;
    normal.normalize();
    return normal;
}
