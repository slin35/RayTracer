#pragma once

#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "Camera.h"
#include "Sphere.h"
#include "Pigment.h"
#include "Ray.h"
#include "Light.h"
#include "Plane.h"
#include "Util.h"
#include "Object.h"
#include "Hit.h"
#include "Shape.h"

using namespace std;

class Scene {
    public:
        Scene(int width, int height, int renderMode, int numRays, int bounces) 
            : width(width), height(height), renderMode(renderMode), numRays(numRays), bounces(bounces) {
                buffer = vector<vector<Pigment>>(width, vector<Pigment>(height, Pigment()));
            }

        vector<shared_ptr<Camera>> getCameras() { return cameras; }
        vector<shared_ptr<Sphere>> getSpheres() { return spheres; }

        void addCamera(shared_ptr<Camera> camera) {
            cameras.push_back(camera);
        }

        void addSphere(shared_ptr<Sphere> sphere) {
            spheres.push_back(sphere);
            objects.push_back(sphere);
        }

        void addLight(shared_ptr<Light> light) {
            lights.push_back(light);
            if (light->isAreaLight()) {
                objects.push_back(light);
                areaLight = true;
            }
        }

        void addPlane(shared_ptr<Plane> plane) {
            planes.push_back(plane);
            objects.push_back(plane);
        }

        void addShape(shared_ptr<Shape> shape) {
            shapes.push_back(shape);
            objects.push_back(shape);
        }

        void render(ostream& out);

    public:
        int width;
        int height;
        int renderMode;
        int numRays;
        int bounces;
        bool areaLight = false;
        vector<shared_ptr<Camera>> cameras;
        vector<shared_ptr<Sphere>> spheres;
        vector<shared_ptr<Light>> lights;
        vector<shared_ptr<Plane>> planes;
        vector<shared_ptr<Shape>> shapes;
        vector<shared_ptr<Object>> objects;
        Pigment inside{0, 0, 0};
        Pigment outside{0.6, 0.8, 0.3};
        vector<vector<Pigment>> buffer;
        
        void writeOutHeader(ostream& out);
        void writeOutPixel(ostream& out, int xpos, int ypos, Pigment color, bool disAttenuation);
        Pigment traceColor(int x, int y);
        void renderBuffer(ostream& out);
};

void Scene::render(ostream& out) {
    bool flag = false;
    double res;

    writeOutHeader(out);

    // if there's no light and the render mode requires lighting, shading model is cleared
    if (lights.size() == 0 && renderMode == 0)
        renderMode = -1;


    #pragma omp parallel for num_threads(2)
    for (int y = height - 1; y >= 0; y--) {
        cerr << "\r rows remaining:  " << y << "            " << flush;
        for (int x = 0; x < width; x++) {
            buffer[x][y] = traceColor(x, y);
        }
    } 

    renderBuffer(out);

}

// fill in header
void Scene::writeOutHeader(ostream& out) {
    out << "P3\n";
    out << width << " " << height << "\n";
    out << 255 << "\n";
}

// fill in pixel value
void Scene::writeOutPixel(ostream& out, int xpos, int ypos, Pigment color, bool disAttenuation) {
    float d;

    if (disAttenuation)
        d = 1;
    else
        d = 1;
    
    Util::gammaEncoder(color, 2);
    color = color * 255 * d;
    
    out << min((int)color.r, 255) << " " << min((int)color.g, 255) << " " << min((int)color.b, 255) << " ";
}


Pigment Scene::traceColor(int x, int y) {
    Pigment color;
    Ray ray;
    double res;

    if (areaLight) {
        outside = Pigment{0, 0, 0};
    }
    // for every pixel generate rays with position(x +/- 0.5, y +/- 0.5)
    for (int i = 0; i < numRays; i++) {
        ray = Ray(cameras[0], x + Util::randD(-0.5), y + Util::randD(-0.5), width, height);

        shared_ptr<Object> obj;
        Hit hit(&objects, ray);

        res = hit.getClosestHit(obj);

        if (hit.isHit()) {
            switch (renderMode)
                {
                case 0:
                    color = color + Util::phongMode(&lights, &objects, obj, ray.getCurrentPos(res));
                    break;
                case 1:
                    color = color + Util::foggyMode(ray, &objects, outside, bounces);
                    break;
                default:
                    color = color + obj->getColor();
                    break;
                }
        }
        else {
            color = color + outside;
        }
    }
    color = color / numRays;
    return color;
}


void Scene::renderBuffer(ostream& out) {
    for (int y = height - 1; y >= 0; y--) {
        
        for (int x = 0; x < width; x++) {
            writeOutPixel(out, x, y, buffer[x][y], false);
        }
        out << "\n";
    }
}