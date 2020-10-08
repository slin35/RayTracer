#pragma once

#include <vector>

#include "Camera.h"
#include "Sphere.h"
#include "Pigment.h"
#include "Ray.h"
#include "Light.h"
#include "Plane.h"
#include "Util.h"

#define N 1

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
        }

        void addLight(shared_ptr<Light> light) {
            lights.push_back(light);
        }

        void addPlane(shared_ptr<Plane> plane) {
            planes.push_back(plane);
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
        Pigment inside{0, 0, 0};
        Pigment outside{0.6, 0.8, 0.3};
        
        void writeOutHeader(ostream& out);
        void writeOutPixel(ostream& out, int xpos, int ypos, Ray ray, Pigment color, bool disAttenuation);
        
};

void Scene::render(ostream& out) {
    bool flag = false;
    double res;

    writeOutHeader(out);

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Ray ray;
            Pigment inside;

            for (int i = 0; i < N; i++) {
                ray = Ray(cameras[0], x + Util::randD(-0.5), y + Util::randD(-0.5), width, height);
                for (auto sphere : spheres) {
                    res = ray.hit(sphere);
                    if (res > 0) {
                        flag = true;
                        vec3 curPos = ray.getCurrentPos(res);
                        vec3 normal = curPos - sphere->getCenter();
                        vec3 light = lights[0]->getPosition() - curPos;
                        inside = inside + sphere->getpigment() * lights[0]->getPigment() * Util::phongDiffuse(1, normal, light);
                        break;
                    }
                }
            } 
            inside = inside / N;

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

    out << (int)(255 * color.r * d) << " " << (int)(255 * color.g * d) << " " << (int)(255 * color.b * d)<< " ";
}