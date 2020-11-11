#pragma once

#include <vector>
#include <iostream> 
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>


#include "Pigment.h"
#include "vec3.h"
#include "Object.h"
#include "Util.h"

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
        void setScale(vec3 scale) { 
            this->scale = scale;
            S = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x(), scale.y(), scale.z()));
            ctm = S * ctm;
        }
        void setRotate(vec3 rotate) { 
            this->rotate = rotate;
            glm::vec3 axis(0.0f);
            float angle = 0.0f;
            if (rotate.x() != 0) {
                axis.x = 1.0;
                angle = rotate.x();
            }
            else if (rotate.y() != 0) {
                axis.y = 1.0;
                angle = rotate.y();
            }
            else if (rotate.z() != 0) {
                axis.z = 1.0;
                angle = rotate.z();
            }
            angle = (float)angle / 180.0 * 3.14;
            R = glm::rotate(glm::mat4(1.0f), angle, axis);
            ctm = R * ctm;
        }
        void setTranslate(vec3 translate) { 
            this->translate = translate; 
            T = glm::translate(glm::mat4(1.0f), glm::vec3(translate.x(), translate.y(), translate.z()));
            ctm = T * ctm;
        }

        void transCTM() {
            invCTM = glm::inverse(ctm);
            invTransCTM = glm::inverseTranspose(ctm);
            transposeCTM = glm::transpose(ctm);
        }
        void setTextureImg(string img) {
            this->img = img;
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
        virtual void setSurfaceType(int type) { this->type = type; }


    private:
        vec3 center;
        double radius = 0;
        Pigment pigment{1.0f, 1.0f, 1.0f};
        double fuzzy = 0;
        int type = 0;
        double ior;
        vec3 scale;
        vec3 rotate;
        vec3 translate;
        glm::mat4 ctm = glm::mat4(1.0f);
        glm::mat4 invCTM = glm::mat4(1.0f);
        glm::mat4 invTransCTM = glm::mat4(1.0f);
        glm::mat4 transposeCTM = glm::mat4(1.0f);
        glm::mat4 S = glm::mat4(1.0f);
        glm::mat4 R = glm::mat4(1.0f);
        glm::mat4 T = glm::mat4(1.0f);
        string img;
        
};



// returns the t value along the ray direction intersecting the sphere
double Sphere::hit(Ray ray) {
    vec3 e = ray.position;
    vec3 d = ray.direction;

    e = Util::applyCTM(e, invCTM, 1.0);
    d = Util::applyCTM(d, invCTM, 0.0);

    double r = radius;

    double a = d.dot(d);
    double b = 2.0 * d.dot(e - center);
    double c = (e - center).dot(e - center) - r * r;
    
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0)
        return -1;

    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

    if (this->getSurfaceType() == 3) {
        if ((t1 > 0 && t2 < 0) || (t1 < 0 && t2 > 0))
            return max(t1, t2); 
    }

    return min(t1, t2);
}

vec3 Sphere::getN(vec3 curPos) {
    vec3 p = Util::applyCTM(curPos, invCTM, 1.0);
    vec3 normal = p - center;
    normal.normalize();

    normal = Util::applyCTM(normal, invTransCTM, 0.0);
    normal.normalize();
    return normal;
}
