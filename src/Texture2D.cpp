//
// Created by Scott on 2022/5/21.
//

#include "Texture2D.h"

#define STBI_FAILURE_USERMSG

Texture2D::Texture2D(const string &path, bool flip, GLenum format) {
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(path.c_str(), &this->width, &this->height,
                                    &this->color_channels, 0);
    if (!data) {
        throw std::runtime_error(stbi_failure_reason());
    }
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    // default parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height,
                 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Texture2D::bind(GLenum tex_unit) const {
    glActiveTexture(tex_unit);
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::set_parameter(GLenum parameter, int value) {
    glTexParameteri(GL_TEXTURE_2D, parameter, value);
}

