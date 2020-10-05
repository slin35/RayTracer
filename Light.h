#pragma once

#include "vec3.h"
#include "Pigment.h"

class Light {
    public:
        Light() {}

        Light(vec3 position, Pigment pigment) :
            position(position), pigment(pigment) {}

        vec3 getPosition() { return position; }
        Pigment getPigment() { return pigment; }

        void setPosition(vec3 position) { this->position = position; }
        void setPigment(vec3 pigment) { this->pigment.setPigment(pigment); }

    private:
        vec3 position;
        Pigment pigment{0.0f, 0.0f, 0.0f};
};