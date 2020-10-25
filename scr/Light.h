#pragma once

#include "vec3.h"
#include "Pigment.h"
#include "Object.h"
#include "Ray.h"


class Light : public Object {
    public:
        Light() {}

        Light(vec3 position, Pigment pigment) :
            position(position), pigment(pigment) {}

        vec3 getPosition() { return position; }
        Pigment getPigment() { return pigment; }
        vec3 getV1() { return v1; }
        vec3 getV2() { return v2; }
        bool isAreaLight() { return areaLight; }

        void setPosition(vec3 position) { this->position = position; }
        void setPigment(vec3 pigment) { this->pigment.setPigment(pigment); }
        void setV1(vec3 v1) { this->v1 = v1; this->areaLight = true; }
        void setV2(vec3 v2) { this->v2 = v2; }

        virtual double hit(Ray ray);
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0));
        virtual Pigment getColor() { return pigment; }
        virtual int getSurfaceType() { return 2; }

    private:
        vec3 position;
        Pigment pigment{0.0f, 0.0f, 0.0f};
        vec3 v1;
        vec3 v2;
        bool areaLight = false;
};


double Light::hit(Ray ray) {
    double t;
    vec3 curPos;

    if (position.y() != 0) {
        t = (position.y() - ray.position.y()) / ray.direction.y();

        curPos = ray.getCurrentPos(t);

        double x0 = position.x() - v1.x() * 0.5 - v2.x() * 0.5;
        double x1 = position.x() + v1.x() * 0.5 + v2.x() * 0.5;
        double z0 = position.z() - v1.z() * 0.5 - v2.z() * 0.5;
        double z1 = position.z() + v1.z() * 0.5 + v2.z() * 0.5;

        if (curPos.x() < x0 || curPos.x() > x1 || curPos.z() < z0 || curPos.z() > z1) {
            return -1;
        }
    }
    if (position.x() != 0) {
        t = (position.x() - ray.position.x()) / ray.direction.x();

        curPos = ray.getCurrentPos(t);

        double y0 = position.y() - v1.y() * 0.5 - v2.y() * 0.5;
        double y1 = position.y() + v1.y() * 0.5 + v2.y() * 0.5;
        double z0 = position.z() - v1.z() * 0.5 - v2.y() * 0.5;
        double z1 = position.z() + v1.z() * 0.5 + v2.z() * 0.5;

        if (curPos.y() < y0 || curPos.y() > y1 || curPos.z() < z0 || curPos.z() > z1) {
            return -1;
        }

    }
    if (position.z() != 0) {
        t = (position.z() - ray.position.z()) / ray.direction.z();

        curPos = ray.getCurrentPos(t);

        double x0 = position.x() - v1.x() * 0.5 - v2.x() * 0.5;
        double x1 = position.x() + v1.x() * 0.5 + v2.x() * 0.5;
        double y0 = position.y() - v1.y() * 0.5 - v2.y() * 0.5;
        double y1 = position.y() + v1.y() * 0.5 + v2.y() * 0.5;

        if (curPos.x() < x0 || curPos.x() > x1 || curPos.y() < y0 || curPos.y() > y1) {
            return -1;
        }
    }

    return t;
}

vec3 Light::getN(vec3 curPos) {
    vec3 normal = position;
    normal.normalize();
    return normal;
}
