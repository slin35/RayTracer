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

using namespace std;

class Scene {
    public:
        Scene(int width, int height, int renderMode, int numRays, int bounces) 
            : width(width), height(height), renderMode(renderMode), numRays(numRays), bounces(bounces) {}

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
        int numRays;
        int bounces;
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

    // if there's no light and the render mode requires lighting, shading model is cleared
    if (lights.size() == 0 && renderMode == 0)
        renderMode = -1;

    for (int y = height - 1; y >= 0; y--) {
       // cerr << "\r rows remaining:" << y << " " << flush;
        for (int x = 0; x < width; x++) {
            Ray ray;
            Pigment color;
            
            // for every pixel generate rays with position(x +/- 0.5, y +/- 0.5)
            for (int i = 0; i < numRays; i++) {
                ray = Ray(cameras[0], x + Util::randD(-0.5), y + Util::randD(-0.5), width, height);

                // check if the ray hits any geometries
                for (auto o : objects) {
                    res = o->hit(ray);

                    // if the ray hits a geometry, redner with given shading model
                    if (res > 0) { 
                        flag = true;
                        switch (renderMode)
                        {
                        case 0:
                            color = color + Util::phongMode(lights, objects, o, ray.getCurrentPos(res));
                            break;
                        case 1:
                            color = color + Util::foggyMode(lights, o, ray, objects, outside, bounces);
                            break;
                        default:
                            color = color + o->getColor();
                            break;
                        }
                        break;
                    }
                }
                
                // if not hit, returns the background color
                if (!flag) {
                    color = color + outside;
                }
                else {
                    flag = false;
                }
            }

            color = color / numRays;

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
    
    Util::gammaEncoder(color, 2);
    color = color * 255 * d;
    
    out << (int)color.r << " " << (int)color.g << " " << (int)color.b << " ";
}
