#pragma once


#include <vector>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

class Shape {
    public:
        Shape(string inputFile) {
            if (!tinyobj::LoadObj(TOshapes, objMaterials, err, inputFile.c_str())) {
                throw runtime_error(err);
            }
        }

    public:
        vector<tinyobj::shape_t> TOshapes;
        vector<tinyobj::material_t> objMaterials;
        string err;

        vector<unsigned int> eleBuf;
        vector<float> posBuf;
        vector<float> norBuf;
        vector<float> texBuf;

        glm::vec3 min = glm::vec3(0);
        glm::vec3 max = glm::vec3(0);

        void initShape(int shapeIdx);
        void measure();
        void resize(glm::vec3 gTrans, float gScale);
        void setNormals();

};

void Shape::initShape(int shapeIdx) {
    tinyobj::shape_t shape = TOshapes[shapeIdx];

    posBuf = shape.mesh.positions;
    norBuf = shape.mesh.normals;
    texBuf = shape.mesh.texcoords;
    eleBuf = shape.mesh.indices;

    if (norBuf.size() == 0)
        setNormals();

    measure();

    glm::vec3 gTrans = min + 0.5f * (max - min);
    float gScale;

    if (max.x > max.y && max.x > max.z) {
        gScale = 2.0 / (max.x - min.x);
    }
    else if (max.y > max.x && max.y > max.z) {
        gScale = 2.0 / (max.y - min.y);
    }
    else {
        gScale = 2.0 / (max.z - min.z);
    }

    resize(gTrans, gScale);

    measure();
}

void Shape::measure() {
    float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = minY = minZ = std::numeric_limits<float>::infinity();
	maxX = maxY = maxZ = -std::numeric_limits<float>::infinity();

	//Go through all vertices to determine min and max of each dimension
	for (size_t v = 0; v < posBuf.size() / 3; v++)
	{
		if (posBuf[3*v+0] < minX) minX = posBuf[3 * v + 0];
		if (posBuf[3*v+0] > maxX) maxX = posBuf[3 * v + 0];

		if (posBuf[3*v+1] < minY) minY = posBuf[3 * v + 1];
		if (posBuf[3*v+1] > maxY) maxY = posBuf[3 * v + 1];

		if (posBuf[3*v+2] < minZ) minZ = posBuf[3 * v + 2];
		if (posBuf[3*v+2] > maxZ) maxZ = posBuf[3 * v + 2];
	}

	min.x = minX;
	min.y = minY;
	min.z = minZ;
	max.x = maxX;
	max.y = maxY;
	max.z = maxZ;
}

void Shape::resize(glm::vec3 gTrans, float gScale) {
	for (size_t v = 0; v < posBuf.size() / 3; v++) {
		posBuf[3 * v + 0] = (posBuf[3 * v + 0] - gTrans.x) * gScale;
		posBuf[3 * v + 1] = (posBuf[3 * v + 1] - gTrans.y) * gScale;
		posBuf[3 * v + 2] = (posBuf[3 * v + 2] - gTrans.z) * gScale;
	}
}

void Shape::setNormals() {
	float ax, ay, az, bx, by, bz, cx, cy, cz, nx, ny, nz;

	for (size_t v = 0; v < posBuf.size() / 3; v++) {
		norBuf.push_back(0);
		norBuf.push_back(0);
		norBuf.push_back(0);
	}

	for (size_t v = 0; v < eleBuf.size() / 3; v++) {
		ax = posBuf[3 * eleBuf[3 * v]];
		ay = posBuf[3 * eleBuf[3 * v] + 1];
		az = posBuf[3 * eleBuf[3 * v] + 2];

		bx = posBuf[3 * eleBuf[3 * v + 1]];
		by = posBuf[3 * eleBuf[3 * v + 1] + 1];
		bz = posBuf[3 * eleBuf[3 * v + 1] + 2];

		cx = posBuf[3 * eleBuf[3 * v + 2]];
		cy = posBuf[3 * eleBuf[3 * v + 2] + 1];
		cz = posBuf[3 * eleBuf[3 * v + 2] + 2];

		nx = (by - ay)*(cz - az) - (bz - az)*(cy - ay);
		ny = (bz - az)*(cx - ax) - (bx - ax)*(cz - az);
		nz = (bx - ax)*(cy - ay) - (by - ay)*(cx - ax);

		norBuf[3 * eleBuf[3 * v]] += nx;
		norBuf[3 * eleBuf[3 * v] + 1] += ny;
		norBuf[3 * eleBuf[3 * v] + 2] += nz;

		norBuf[3 * eleBuf[3 * v + 1]] += nx;
		norBuf[3 * eleBuf[3 * v + 1] + 1] += ny;
		norBuf[3 * eleBuf[3 * v + 1] + 2] += nz;

		norBuf[3 * eleBuf[3 * v + 2]] += nx;
		norBuf[3 * eleBuf[3 * v + 2] + 1] += ny;
		norBuf[3 * eleBuf[3 * v + 2] + 2] += nz;
	}
}

/*


    public:
        Shape(string inputfile) {
            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str())) {
                throw runtime_error(warn + err);
            }

        }
    public:
        tinyobj::attrib_t attrib;
        vector<tinyobj::shape_t> shapes;
        vector<tinyobj::material_t> materials;
        string warn;
        string err;

        vector<float> norBuf;
        vector<float> posBuf;
        vector<float> texBuf;
        vector<tinyobj::index_t> eleBuf;
        vector<glm::vec3> vertices;
        vector<glm::vec3> normals;

        glm::vec3 min = glm::vec3(0);
        glm::vec3 max = glm::vec3(0);

        void setup(size_t shapeIdx = 0);
        void setNormal();


void Shape::setup(size_t shapeIdx) {
    posBuf = attrib.vertices;
    norBuf = attrib.normals;
    texBuf = attrib.texcoords;

    for (const auto& shape: shapes) {
        for (const auto& index : shape.mesh.indices) {
            eleBuf.push_back(index);     // setup eleBuf (might not be necessary)
            vertices.push_back(glm::vec3(posBuf[3*index.vertex_index+0], posBuf[3*index.vertex_index+1], posBuf[3*index.vertex_index+2]));
        }
    }
}

void Shape::setNormal() {
    float ax, ay, az, bx, by, bz, cx, cy,cz, nx, ny, nz;

    norBuf.resize(posBuf.size() / 3);
}
 */

