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

    public:
        vec3 position;
        vec3 direction;

        void setup(shared_ptr<Camera> camera, float xPos, float yPos, float width, float height);

};

// setup the ray given a camera position, (x, y) pixel position, and the screen size
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