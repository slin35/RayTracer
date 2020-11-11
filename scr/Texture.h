#pragma once

#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

class Texture {
    public:
        Texture() {}

        Texture(string img) {
            createTexture(img);
        }

    public:
        int width;
        int height;
        int numComp;
        unsigned char *data;

        void createTexture(string img) {
            // load texture
            stbi_set_flip_vertically_on_load(true);
            data = stbi_load(img.c_str(), &width, &height, &numComp, 0);

            if (!data) {
                cerr << img << " not found" << endl;
            }
            if (numComp != 3) {
                cerr << img << " must have 3 components (RGB)" << endl;
            }
            if ((width & (width - 1)) != 0 || (height & (height - 1)) != 0) {
                cerr << img << " must be a power of 2" << endl;
            }
        }
};