#pragma once

#include <vector>
#include <iterator>
#include <algorithm>

#include "Camera.h"
#include "Sphere.h"
#include "Pigment.h"
#include "Ray.h"
#include "Light.h"
#include "Plane.h"
#include "Util.h"
#include "Object.h"

#define N 20

using namespace std;

class Scene {
    public:
        Scene(int width, int height, int renderMode) 
            : width(width), height(height), renderMode(renderMode) {}

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
        }

        void addPlane(shared_ptr<Plane> plane) {
            planes.push_back(plane);
            objects.push_back(plane);
        }

        void render(ostream& out);

    public:
        int width;
        int height;
        int renderMode;
        vector<shared_ptr<Camera>> cameras;
        vector<shared_ptr<Sphere>> spheres;
        vector<shared_ptr<Light>> lights;
        vector<shared_ptr<Plane>> planes;
        vector<shared_ptr<Object>> objects;
        Pigment inside{0, 0, 0};
        Pigment outside{0.6, 0.8, 0.3};
        
        void writeOutHeader(ostream& out);
        void writeOutPixel(ostream& out, int xpos, int ypos, Pigment color, bool disAttenuation);
        
};

void Scene::render(ostream& out) {
    bool flag = false;
    double res;

    writeOutHeader(out);

    if (lights.size() == 0 && renderMode != 1)
        renderMode = -1;

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Ray ray;
            Pigment color;

            for (int i = 0; i < N; i++) {
                ray = Ray(cameras[0], x + Util::randD(-0.5), y + Util::randD(-0.5), width, height);

                for (auto o : objects) {
                    res = o->hit(ray);
                    if (res > 0) {
                        flag = true;
                        switch (renderMode)
                        {
                        case -1:
                            color = color + o->getColor();
                            break;
                        case 0:
                            color = color + Util::phongMode(lights, objects, o, ray.getCurrentPos(res));
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                }

                if (!flag) {
                    color = color + outside;
                }
                else {
                    flag = false;
                }
            }

            color = color / N;

            writeOutPixel(out, x, y, color, false);
        }
        out << "\n";
    }

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
    
    Util::gammaEncoder(color, 1.5);
    color = color * 255 * d;
    
    out << (int)color.r << " " << (int)color.g << " " << (int)color.b << " ";
}