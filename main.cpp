#include <iostream>
#include <fstream>
#include <vector>

#include "Parser.h"
#include "Camera.h"
#include "Sphere.h"
#include "Scene.h"

using namespace std;

void readInts(istream& is, vector<int>& res) {
	int inI;
	while (is >> inI) {
		res.push_back(inI);
	}
}

int main(int argc, char *argv[]) {
	int width, height, shadingModel, numRays, bounces;
	ifstream inFile;
	ofstream outFile;

	if (argc < 8) {
		cerr << "Error format: a.out inFile outFile" << endl;
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
			Parser p(inFile, scene);

			if (outFile) {
				scene.render(outFile);
			}
			else {
				cerr << "error writing file" << endl;
			}
		}
	}

}
