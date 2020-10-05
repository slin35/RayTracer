#pragma once

#include <vector>

#include "Object.h"
#include "vec3.h"

using namespace std;

class Camera : public Object {
    public:
        Camera() {}

        Camera(vec3& eye, vec3& up, vec3& right, vec3& lookAt) :
            eye(eye), up(up), right(right), lookAt(lookAt) {}

        void setEye(vec3 eye) { this->eye = eye; }
        void setUp(vec3 up) { this->up = up; }
        void setRight(vec3 right) { this->right = right; }
        void setLookAt(vec3 lookAt) { this->lookAt = lookAt; }

        vec3 getEye() const { return eye; }
        vec3 getUp() const { return up; }
        vec3 getRight() const { return right; }
        vec3 getLookAt() const { return lookAt; }

    private:
        vec3 eye{0, 0, 0};
        vec3 up{0, 1, 0};
        vec3 right{1, 0, 0};
        vec3 lookAt{0, 0, 0};
};