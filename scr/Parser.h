#pragma once

#include <regex>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <istream>


#include "Camera.h"
#include "Sphere.h"
#include "Scene.h"

using namespace std;

#define CAMERA "camera"
#define SPHERE "sphere"
#define LIGHTSOURCE "light_source"
#define PLANE "plane"
#define START "{"
#define END "}"


class Parser {
 
    public:
        Parser(istream& input, Scene& scene) : input(input), scene(scene) {
            parse();
        }

    private:
        istream& input;
        Scene& scene;

        void parse() {
            string w;

            while (input >> w) {
                if (w.compare(CAMERA) == 0) {
                    parseCamera();
                }
                if (w.compare(SPHERE) == 0) {
                    parseSphere();
                }
                if (w.compare(LIGHTSOURCE) == 0) {
                    parseLight();
                }
                if (w.compare(PLANE) == 0) {
                    parsePlane();
                }
            }
        }

        // parse camera data and add to list of cameras in the scene
        void parseCamera() {
            string w;
            vec3 tmp;

            auto camera = make_shared<Camera>();

            while (input >> w) {
                if (w.compare(END) == 0) {
                    break;
                }

                if (w.compare("location") == 0) {
                    tmp.setX(extractDouble(w));
                    tmp.setY(extractDouble(w));
                    tmp.setZ(extractDouble(w));

                    camera->setEye(tmp);
                }
                
                if (w.compare("up") == 0) {
                    tmp.setX(extractDouble(w));
                    tmp.setY(extractDouble(w));
                    tmp.setZ(extractDouble(w));

                    camera->setUp(tmp);
                }

                if (w.compare("right") == 0) {
                    tmp.setX(extractDouble(w));
                    tmp.setY(extractDouble(w));
                    tmp.setZ(extractDouble(w));

                    camera->setRight(tmp);
                }

                if (w.compare("look_at") == 0) {
                    tmp.setX(extractDouble(w));
                    tmp.setY(extractDouble(w));
                    tmp.setZ(extractDouble(w));

                    camera->setLookAt(tmp);
                }
            }
            scene.addCamera(camera);
        }

        // parse sphere data and add it to the list of geometries in the scene
        void parseSphere() {
            string w;
            char c;
            vec3 tmp;
            auto sphere = make_shared<Sphere>();

            while (input >> w) {
                if (w.compare(END) == 0) {
                    break;
                }

                tmp.setX(extractDouble(w));
                tmp.setY(extractDouble(w));
                tmp.setZ(extractDouble(w));

                sphere->setCenter(tmp);
                sphere->setRadius(extractDouble(w));

                input >> w;
                
                if (w.compare("pigment") == 0) {
                    input >> w;
                    input >> w;
                    if (w.compare("color") == 0) {
                        input >> w;  
                        if (w.compare("rgb") == 0) {
                            tmp.setX(extractDouble(w));
                            tmp.setY(extractDouble(w));
                            tmp.setZ(extractDouble(w));

                            sphere->setPigment(tmp);
                        }
                    }
                }

                input >> w;
                if (w.compare(END) == 0) {
                    break;
                }
                if (w.compare("finish") == 0) {
                    input >> c;
                    input >> w;
                    if (w.compare("reflection") == 0) {
                        sphere->setFuzzy(extractDouble(w));
                    }
                }



                
            }
            scene.addSphere(sphere);
        }

        // parse the light source data and add to list of lights in scene
        void parseLight() {
            char c;
            string w;
            vec3 tmp;
            auto light = make_shared<Light>();

            while (input >> c && c != '}') {
                if (c == '{') {
                    tmp.setX(extractDouble(w));
                    tmp.setY(extractDouble(w));
                    tmp.setZ(extractDouble(w));

                    light->setPosition(tmp);
                }

                input >> w;
                if (w.compare("color") == 0) {
                    input >> w;
                    if (w.compare("rgb") == 0) {
                        tmp.setX(extractDouble(w));
                        tmp.setY(extractDouble(w));
                        while (input >> c && c != '>') {
                            w += c;
                        }
                        tmp.setZ(stod(w));
                        light->setPigment(tmp);
                    }
                }

                input >> c;
                if (c != '}') {
                    input >> w;
                    w = c + w;
                    if (w.compare("area_light") == 0) {
                        tmp.setX(extractDouble(w));
                        tmp.setY(extractDouble(w));
                        tmp.setZ(extractDouble(w));
                        light->setV1(tmp);

                        tmp.setX(extractDouble(w));
                        tmp.setY(extractDouble(w));
                        tmp.setZ(extractDouble(w));
                        light->setV2(tmp);
                        
                    }
                }
                else {
                    break;
                }
            }

            scene.addLight(light);
        }

        // parse plane data and add it to the list of geometries in the scene
        void parsePlane() {
            char c;
            string w;
            vec3 tmp;
            auto plane = make_shared<Plane>();

            while (input >> c && c != '}') {
                if (c == '{') {
                    tmp.setX(extractDouble(w));
                    tmp.setY(extractDouble(w));
                    tmp.setZ(extractDouble(w));

                    plane->setNormal(tmp);
                    plane->setDistance(extractDouble(w));
                }

                input >> w;
                if (w.compare("pigment") == 0) {
                    input >> c;
                    input >> w;
                    if (w.compare("color") == 0) {
                        input >> w;
                        if (w.compare("rgb") == 0) {
                            tmp.setX(extractDouble(w));
                            tmp.setY(extractDouble(w));
                            tmp.setZ(extractDouble(w));

                            plane->setPigment(tmp);
                            break;
                        }
                    }
                    
                }
            }
            scene.addPlane(plane);
        }

        // extracting integers of decimals from a srting
        double extractDouble(string& w) {
            regex target("([-]*[0-9]+[.]*[0-9]*[e]*[-]*[0-9]*)");
            smatch match;

            input >> w;
            if (regex_search(w, match, target)) {
                return stod(match.str());
            }

            return -1;
        }

};