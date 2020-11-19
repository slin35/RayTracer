#pragma once


#include <vector>
#include <string>
#include <limits>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Object.h"
#include "Pigment.h"
#include "Ray.h"
#include "vec3.h"
#include "Triangle.h"
#include "Util.h"
#include "Object.h"

using namespace std;

class Shape : public Object {
    public:
        Shape() {}

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

        vector<shared_ptr<Triangle>> triangles;

        float tmin = 0;
        float tmax = 0;
        vec3 hitNormal;
        
        Pigment color{0.6, 0.6, 0.8};
        glm::mat4 ctm = glm::mat4(1.0f);
        glm::mat4 invCTM = glm::mat4(1.0f);
        glm::mat4 invTransCTM = glm::mat4(1.0f);
        glm::mat4 transposeCTM = glm::mat4(1.0f);
        glm::mat4 S;
        glm::mat4 T;
        glm::mat4 R;

        void initShape(int shapeIdx);
        void measure();
        void resize(glm::vec3 gTrans, float gScale);

        void setColor(vec3 color) {
            this->color = Pigment(color);
        }
        void setScale(vec3 scale) {
            S = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x(), scale.y(), scale.z()));
            ctm = S * ctm;
        }
        void setTranslate(vec3 translate) {
            T = glm::translate(glm::mat4(1.0f), glm::vec3(translate.x(), translate.y(), translate.z()));
            ctm = T * ctm;
        }
        void setRotate(vec3 rotate) {
            glm::vec3 axis(0.0f);
            float angle = 0.0f;
            if (rotate.x() != 0) {
                axis.x = 1.0;
                angle = rotate.x();
            }
            else if (rotate.y() != 0) {
                axis.y = 1.0;
                angle = rotate.y();
            }
            else if (rotate.z() != 0) {
                axis.z = 1.0;
                angle = rotate.z();
            }
            angle = (float)angle / 180.0 * 3.14;
            R = glm::rotate(glm::mat4(1.0f), angle, axis);
            ctm = R * ctm;
        }
        void transCTM() {
            invCTM = glm::inverse(ctm);
            invTransCTM = glm::inverseTranspose(ctm);
            transposeCTM = glm::transpose(ctm);
            for (auto tri : triangles) {
                tri->setCTM(ctm);
            }
        }


        bool hitBoundingBox(Ray ray);
        void addTriangles();

        virtual double hit(Ray ray);
        virtual Pigment getColor() { return this->color; }
        virtual vec3 getN(vec3 curPos = vec3(0, 0, 0));

};

double Shape::hit(Ray ray) {
    if (hitBoundingBox(ray)) {
        vector<shared_ptr<Object>> objs(triangles.begin(), triangles.end());
        shared_ptr<Object> hitObj;
        Hit hit(&objs, ray);

        double res = hit.getClosestHit(hitObj);
        if (res >= 0) {
            hitNormal = hitObj->getN();
        }
        return res;

    }
    return -1;
}

vec3 Shape::getN(vec3 curPos) {
    return hitNormal;
}

bool Shape::hitBoundingBox(Ray ray) {
    ray.position = Util::applyCTM(ray.position, invCTM, 1.0f);
    ray.direction = Util::applyCTM(ray.direction, invCTM, 0.0f);
    
    glm::vec3 dir = glm::vec3(ray.direction.x(), ray.direction.y(), ray.direction.z());
    glm::vec3 eye = glm::vec3(ray.position.x(), ray.position.y(), ray.position.z());
    glm::vec3 invDir = glm::vec3(1.0/dir.x, 1.0/dir.y, 1.0/dir.z);

    float tymin, tymax, tzmin, tzmax;

    if (dir.x <= 0) {
        tmin = (max.x - eye.x) * invDir.x;
        tmax = (min.x - eye.x) * invDir.x;
    }
    else {
        tmin = (min.x - eye.x) * invDir.x;
        tmax = (max.x - eye.x) * invDir.x;
    }

    if (dir.y <= 0) {
        tymin = (max.y - eye.y) * invDir.y;
        tymax = (min.y - eye.y) * invDir.y;
    }
    else {
        tymin = (min.y - eye.y) * invDir.y;
        tymax = (max.y - eye.y) * invDir.y;
    }

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    if (dir.z <= 0) {
        tzmin = (max.z - eye.z) * invDir.z;
        tzmax = (min.z - eye.z) * invDir.z;
    }
    else {
        tzmin = (min.z - eye.z) * invDir.z;
        tzmax = (max.z - eye.z) * invDir.z;
    }

    if (tmin > tzmax || tzmin > tmax)
        return false;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    if (tmin < 0 && tmax < 0) {
        return false;
    }

    return true;
}

void Shape::initShape(int shapeIdx) {
    tinyobj::shape_t shape = TOshapes[shapeIdx];

    posBuf = shape.mesh.positions;
    norBuf = shape.mesh.normals;
    texBuf = shape.mesh.texcoords;
    eleBuf = shape.mesh.indices;

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

    addTriangles();
}

void Shape::addTriangles() {
    for (size_t idx = 0; idx < eleBuf.size() / 3; idx++) {
        glm::vec3 v1(posBuf[3 * eleBuf[3 * idx]], posBuf[3 * eleBuf[3 * idx] + 1], posBuf[3 * eleBuf[3 * idx] + 2]);
        glm::vec3 v2(posBuf[3 * eleBuf[3 * idx + 1]], posBuf[3 * eleBuf[3 * idx + 1] + 1], posBuf[3 * eleBuf[3 * idx + 1] + 2]);
        glm::vec3 v3(posBuf[3 * eleBuf[3 * idx + 2]], posBuf[3 * eleBuf[3 * idx + 2] + 1], posBuf[3 * eleBuf[3 * idx + 2] + 2]);

        auto triangle = make_shared<Triangle>(v1, v2, v3, min, max);
        
        triangles.push_back(triangle);
    }
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