#pragma once

#include <vector>
#include <algorithm>
#include <limits>

#include "Object.h"
#include "Ray.h"

using namespace std;

class Hit {
    public:
        Hit(vector<shared_ptr<Object>>* objects, Ray ray) : objects(objects), ray(ray) {}

        double getClosestHit(shared_ptr<Object>& object) {
            double res;
            double minHit = numeric_limits<double>::max();

            for (auto o : *objects) {
                res = o->hit(ray);
                if (res > 0) {
                    anyHits = true;
                    if (res < minHit) {
                        minHit = res;
                        object = o;
                    }
                    
                }
            }

            return minHit;
        }

        bool isHit() { return anyHits; }

    private:
        vector<shared_ptr<Object>>* objects;
        Ray ray;
        bool anyHits = false;

};