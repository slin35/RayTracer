#pragma once

#include "Ray.h"

class Object {
    public:
        Object() {}

        virtual double hit(Ray ray) = 0;
};