#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Object.h"
#include "vec3.h"
#include "Ray.h"
#include "Util.h"

class Triangle : public Object {
    public:
        Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 min, glm::vec3 max) 
            : a(a), b(b), c(c), min(min), max(max) {

            normal = cross(b - a, c - a);
            normalize(normal);
        }

        friend ostream& operator<<(ostream& os, const Triangle& tri) {
			os << "<" << tri.a.x << ", " << tri.a.y << ", " << tri.a.z << ">  " << "<" << tri.b.x << ", " << tri.b.y << ", " << tri.b.z << "> "<< "<" << tri.c.x << ", " << tri.c.y << ", " << tri.c.z << "> ";
			return os;
		}

    public:
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 c;
        glm::vec3 normal;


        float tmin;
        float tmax;

        glm::vec3 min;
        glm::vec3 max;

        glm::mat4 ctm = glm::mat4(1.0f);
        glm::mat4 invCTM = glm::mat4(1.0f);
        glm::mat4 invTransCTM = glm::mat4(1.0f);
        glm::mat4 transposeCTM = glm::mat4(1.0f);

        virtual double hit(Ray ray);
        virtual Pigment getColor() { return Pigment(0.6, 0.6, 0.8); }
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0));

        void setCTM(glm::mat4 ctm) {
            this->ctm = ctm;
            invCTM = glm::inverse(ctm);
            invTransCTM = glm::inverseTranspose(ctm);
            transposeCTM = glm::transpose(ctm);
        }

};

vec3 Triangle::getN(vec3 curPos) {
    vec3 N = Util::applyCTM(vec3(normal.x, normal.y, normal.z), invTransCTM, 0.0f);
    return N;
}

double Triangle::hit(Ray ray) {
    ray.position = Util::applyCTM(ray.position, invCTM, 1.0f);
    ray.direction = Util::applyCTM(ray.direction, invCTM, 0.0f);

    glm::vec3 dir(ray.direction.x(), ray.direction.y(), ray.direction.z());
    glm::vec3 eye(ray.position.x(), ray.position.y(), ray.position.z());

    double detA = Util::detMatrix(a - b, a - c, dir);
    double t = Util::detMatrix(a - b, a - c, a - eye) / detA;
    if (t < 0)
        return -1;

    double alpha = Util::detMatrix(a - b, a - eye, dir) / detA;
    if (alpha < 0 || alpha > 1) {
        return -1;
    }

    double beta = Util::detMatrix(a - eye, a - c, dir) / detA;
    if (beta < 0 || beta > (1 - alpha)) {
        return -1;
    }

    return t;
}
