#pragma once

#include <iostream>
#include "vec3.h"

class Pigment {
    public:
        Pigment() {}
        Pigment(double r, double g, double b) : 
            r(r), g(g), b(b) {}
        Pigment(vec3 pigment) : 
            r(pigment.x()), g(pigment.y()), b(pigment.z()) {}

        friend ostream& operator<<(ostream& os, const Pigment& p) {
			os << "<" << p.r << ", " << p.g << ", " << p.b << ">";
			return os;
		}

        void setPigment(vec3 pigment) {
            r = pigment.x();
            g = pigment.y();
            b = pigment.z();
        }

    public:
        double r, g, b;
};