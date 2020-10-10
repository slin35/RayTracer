#pragma once

#include <vector>

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

        vec3 getCenter() const { return center; }
        double getRadius() const { return radius; }
        Pigment getpigment() const { return pigment; }

        virtual double hit(Ray ray);
        virtual Pigment getColor() { return pigment; }

    private:
        vec3 center;
        double radius = 0;
        Pigment pigment{0.0f, 0.0f, 0.0f};

};

double Sphere::hit(Ray ray) {
    vec3 e = ray.position;
    vec3 c = center;
    vec3 d = ray.direction;
    double r = radius;
    double discriminant = pow(d.dot(e - c),2) - d.dot(d) * ((e - c).dot(e - c) - pow(r, 2));

    if (discriminant < 0)
        return -1;

    double t1 = d.dot(e - c) * -1 + sqrt(discriminant) / d.dot(d);
	double t2 = d.dot(e - c) * -1 - sqrt(discriminant) / d.dot(d);

    return t1 < t2 ? t1 : t2;
}

