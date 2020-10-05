#pragma once

#include <vector>

#include "Camera.h"
#include "Sphere.h"
#include "Pigment.h"
#include "Ray.h"


using namespace std;

class Scene {
    public:
        Scene(int width, int height) : width(width), height(height) {}

        vector<shared_ptr<Camera>> getCameras() { return cameras; }
        vector<shared_ptr<Sphere>> getSpheres() { return spheres; }

        void addCamera(shared_ptr<Camera> camera) {
            cameras.push_back(camera);
        }

        void addSphere(shared_ptr<Sphere> sphere) {
            spheres.push_back(sphere);
        }

        void render(ostream& out);

    private:
        int width;
        int height;
        vector<shared_ptr<Camera>> cameras;
        vector<shared_ptr<Sphere>> spheres;
        Pigment inside{0, 0, 0};
        Pigment outside{0.6, 0.8, 0.3};
        
        void writeOutHeader(ostream& out);
        void writeOutPixel(ostream& out, int xpos, int ypos, Ray ray, Pigment color, bool disAttenuation);
        
};

void Scene::render(ostream& out) {
    bool flag = false;
    double res;

    writeOutHeader(out);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Ray ray(cameras[0], x, y, width, height);
            Pigment inside;

            for (auto sphere : spheres) {
                res = ray.hit(sphere);
                if (res > 0) {
                    flag = true;
                    inside = Pigment(sphere->getpigment());
                    break;
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
    vec3 unitDir = ray.direction;
    unitDir.normalize();
    auto t = 0.5 * (unitDir.val[1] + 1);

    if (disAttenuation)
        d = 1;
    else
        d = 1;

    out << (int)(255 * color.r * d) << " " << (int)(255 * color.g * d) << " " << (int)(255 * color.b * d)<< "\n";
}