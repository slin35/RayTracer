#pragma once

#include <vector>
#include <iostream> 


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

        vec3 getCenter() const { return center; }
        double getRadius() const { return radius; }
        Pigment getpigment() const { return pigment; }

        virtual double getFuzzy() { return fuzzy; }

        virtual double hit(Ray ray);
        virtual Pigment getColor() { return pigment; }
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0));
        virtual int getSurfaceType() { return type; }

    private:
        vec3 center;
        double radius = 0;
        Pigment pigment{0.0f, 0.0f, 0.0f};
        double fuzzy = 0;
        int type = 0;

};

// returns the t value along the ray direction intersecting the sphere
double Sphere::hit(Ray ray) {
    vec3 e = ray.position;
    vec3 c = center;
    vec3 d = ray.direction;

    d.normalize();
    double r = radius;
    double discriminant = pow(d.dot(e - c),2) - d.dot(d) * ((e - c).dot(e - c) - pow(r, 2));
    
    if (discriminant < 0)
        return -1;

    double t1 = d.dot(e - c) * -1 + sqrt(discriminant) / d.dot(d);
	double t2 = d.dot(e - c) * -1 - sqrt(discriminant) / d.dot(d);

    return t1 < t2 ? t1 : t2;
}

vec3 Sphere::getN(vec3 curPos) {
    return curPos - center;
}
