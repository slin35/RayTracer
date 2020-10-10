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
        void writeOutPixel(ostream& out, int xpos, int ypos, Ray ray, Pigment color, bool disAttenuation);
        
};

void Scene::render(ostream& out) {
    bool flag = false;
    double res;

    writeOutHeader(out);

    if (lights.size() == 0 && renderMode != 1)
        renderMode = -1;

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Ray ray, shadowFeeler;
            Pigment inside;

            for (int i = 0; i < N; i++) {
                ray = Ray(cameras[0], x + Util::randD(-0.5), y + Util::randD(-0.5), width, height);
                
                // rendering spheres
                for (auto sphere : spheres) {
                    res = sphere->hit(ray);
                    if (res > 0) {
                        flag = true;
                        switch (renderMode) {
                        case -1:
                            inside = inside + sphere->getpigment();
                            break;
                        case 0:
                            inside = inside + Util::phongMode(lights, objects, sphere, ray.getCurrentPos(res));
                            break;
                        case 1:
                            inside = inside + Util::foggyMode(sphere, ray, objects, 20);
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                }
            }

            inside = inside / N;

            // rendering plane
            if (!flag) {
                for (auto plane : planes) {
                    res = plane->hit(ray);
                    

                    if (res > 0) {
                        flag = true;
                        switch (renderMode) {
                        case -1:
                            inside = plane->getPigment();
                            break;
                        case 0:
                            inside = inside + Util::phongMode(lights, objects, plane, ray.getCurrentPos(res));
                            break;
                        case 1:
                            inside = plane->getPigment();
                        //    inside = inside + Util::foggyMode(plane, ray, objects, 20);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

            if (flag) {
                writeOutPixel(out, x, y, ray, inside, false);
                flag = false;
            }
            else {
                writeOutPixel(out, x, y, ray, outside, false);
            }

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
void Scene::writeOutPixel(ostream& out, int xpos, int ypos, Ray ray, Pigment color, bool disAttenuation) {
    float d;

    if (disAttenuation)
        d = 1;
    else
        d = 1;
    
    Util::gammaEncoder(color);
    color = color * 255 * d;
    
    out << (int)color.r << " " << (int)color.g << " " << (int)color.b << " ";
}