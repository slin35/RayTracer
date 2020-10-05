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
            }
        }

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

        void parseSphere() {
            string w;
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
                
            }
            scene.addSphere(sphere);
        }

        double extractDouble(string& w) {
            regex target("([0-9]+[.]*[0-9]*)");
            smatch match;

            input >> w;
            if (regex_search(w, match, target)) {
                return stod(match.str());
            }

            return -1;
        }

};