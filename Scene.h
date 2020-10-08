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

#define N 4

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
                        for (auto light : lights) {
                            vec3 curPos = ray.getCurrentPos(res);
                            vec3 normal = curPos - sphere->getCenter();
                            vec3 l = light->getPosition() - curPos;
                            auto it = find(objects.begin(), objects.end(), sphere);

                            shadowFeeler = Ray(curPos, l);
                            if (sphere->inShadow(shadowFeeler, objects, distance(objects.begin(), it)))
                                inside = inside + Pigment(0, 0, 0.1);
                            else
                                inside = inside + sphere->getpigment() * lights[0]->getPigment() * Util::phongDiffuse(1, normal, l);
                            
                        }
                        if (lights.size() == 0)
                            inside = inside + sphere->getpigment();
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
                        for (auto light : lights) {
                            shadowFeeler = Ray(ray.getCurrentPos(res), light->getPosition() - ray.getCurrentPos(res));
                            auto it = find(objects.begin(), objects.end(), plane);
                            if (plane->inShadow(shadowFeeler, objects, distance(objects.begin(), it))) {
                                inside = inside + Pigment(0, 0, 0.1);
                            }
                            else {
                                inside = plane->getPigment();
                            }
                        }
                        if (lights.size() == 0)
                            inside = plane->getPigment();
                        
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

    out << (int)(255 * color.r * d) << " " << (int)(255 * color.g * d) << " " << (int)(255 * color.b * d)<< " ";
}