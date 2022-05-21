//
// Created by Scott on 2022/5/21.
//

#ifndef LEARNOPENGL_TEXTURE2D_H
#define LEARNOPENGL_TEXTURE2D_H

#include <string>
#include <glad/glad.h>
#include <stb_image.h>
#include <stdexcept>

using std::string;

class Texture2D {

public:
    int width = 0;
    int height = 0;
    int color_channels = 0;
    unsigned int id = 0;

    Texture2D(const string &path, bool flip = true, GLenum format = GL_RGBA);
    void bind(GLenum tex_unit = GL_TEXTURE0) const;
    void set_parameter(GLenum parameter, int value);
};


#endif //LEARNOPENGL_TEXTURE2D_H
