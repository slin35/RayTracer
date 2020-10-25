#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "Parser.h"
#include "Camera.h"
#include "Sphere.h"
#include "Scene.h"

#include "Shape.h"

using namespace std;

#define DEBUG 0


int main(int argc, char *argv[]) {
	int width, height, shadingModel, numRays, bounces;
	string objFile;
	ifstream inFile;
	ofstream outFile;

	if (argc < 8) {
		cerr << "Error format: a.out width height inFile outFile shading_model numRays_perPixel recursionDepth" << endl;
	} else {
		width = stoi(argv[1]);
		height = stoi(argv[2]);

		inFile.open(argv[3]);
		cout << "opened: " << argv[3] << endl;
		outFile.open(argv[4]);
		cout << "writing to: " << argv[4] << endl;

		shadingModel = stoi(argv[5]);
		numRays = stoi(argv[6]);
		bounces = stoi(argv[7]);


		if (inFile) {
			Scene scene(width, height, shadingModel, numRays, bounces);
			auto begin = std::chrono::high_resolution_clock::now();
			

			Parser p(inFile, scene);

			auto end = std::chrono::high_resolution_clock::now();
  			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			if (DEBUG)
				cout << "parsing time " << elapsed.count() * 1e-9 << endl;

			if (argc == 9) {
				objFile = argv[8];
				shared_ptr<Shape> shape = make_shared<Shape>(objFile);

				for (int i = 0; i < shape->TOshapes.size(); i++) {
					shape->initShape(i);
					scene.addShape(shape);
				}

			}

			if (outFile) {
				begin = std::chrono::high_resolution_clock::now();
				scene.render(outFile);
				end = std::chrono::high_resolution_clock::now();
  				elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
				if (DEBUG)
					cout << "render time " << elapsed.count() * 1e-9 << endl;
			}
			else {
				cerr << "error writing file" << endl;
			}
		}
	}

}
