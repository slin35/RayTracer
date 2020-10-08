#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "vec3.h"
#include "Camera.h"

using namespace std;

class Ray {
    public:

        Ray() {}

        Ray(vec3 position, vec3 direction) :
            position(position), direction(direction) { direction.normalize(); }

        Ray(shared_ptr<Camera> camera, float xPos, float yPos, float width, float height) {
            setup(camera, xPos, yPos, width, height);
        }

        vec3 getCurrentPos(float time = 0) const{
            return position + direction * time;
        }

      //  double hit(shared_ptr<Sphere> sphere);

    public:
        vec3 position;
        vec3 direction;

        void setup(shared_ptr<Camera> camera, float xPos, float yPos, float width, float height);

};

/*
double Ray::hit(shared_ptr<Sphere> sphere) {
    vec3 e = position;
	vec3 c = sphere->getCenter();
	vec3 d = direction;
	float r = sphere->getRadius();
	float discriminant = pow(d.dot(e - c),2) - d.dot(d) * ((e - c).dot(e - c) - pow(r, 2));

    float t1 = d.dot(e - c) * -1 + sqrt(discriminant) / d.dot(d);
	float t2 = d.dot(e - c) * -1 - sqrt(discriminant) / d.dot(d);

	if (discriminant < 0)
		return -1;
	return t1 < t2 ? t1 : t2;

	float t1 = d.dot(e - c) * -1 + sqrt(discriminant) / d.dot(d);
	float t2 = d.dot(e - c) * -1 - sqrt(discriminant) / d.dot(d);

	vec3 curPos = (ray.getCurrentPos(t1) - ray.getCurrentPos()).leng();
	vec3 curPos2 = (ray.getCurrentPos(t2) - ray.getCurrentPos()).leng();
	return curPos < curPos2 ? curPos : curPos2; 
} */

void Ray::setup(shared_ptr<Camera> camera, float xPos, float yPos, float width, float height) {
    vec3 eye = position = camera->getEye();
    vec3 up = camera->getUp();
    vec3 right = camera->getRight();
    vec3 lookAt = camera->getLookAt();

    vec3 w(eye - lookAt);
    w.normalize();

    vec3 u = up.cross(w);
    vec3 v = w.cross(u);
    vec3 horizontal = u * right.leng();
    vec3 vertical = v * up.leng();

    vec3 llc = position - horizontal * 0.5 - vertical * 0.5 - w;

    direction = llc + horizontal * (xPos / width) + vertical * (yPos / height) - position;
}