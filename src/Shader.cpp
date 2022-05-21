//
// Created by Scott on 2022/5/19.
//

#include "Shader.h"

#include <iostream>

void Shader::init_shader(const unsigned shader_id, const string &path) const {
    ifstream file(path);
    ostringstream source_stream;
    string temp;
    while (std::getline(file, temp)) {
        source_stream << temp << std::endl;
    }
    temp = source_stream.str();
    auto source = temp.c_str();
    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);
    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
        throw std::runtime_error(info_log);
    }
}

Shader::Shader(const string &vertex_shader_path, const string &fragment_shader_path) :
        vertex_shader_id(glCreateShader(GL_VERTEX_SHADER)),
        fragment_shader_id(glCreateShader(GL_FRAGMENT_SHADER)),
        id(glCreateProgram()) {

    this->init_shader(this->vertex_shader_id, vertex_shader_path);
    this->init_shader(this->fragment_shader_id, fragment_shader_path);

    glAttachShader(this->id, this->vertex_shader_id);
    glAttachShader(this->id, this->fragment_shader_id);
    glLinkProgram(this->id);
    // check if linking is successful
    int success;
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(this->id, 512, NULL, info_log);
        throw std::runtime_error(info_log);
    }
    // delete the shaders because we don't need them anymore
//    glDeleteShader(this->vertex_shader_id);
//    glDeleteShader(this->fragment_shader_id);
}

void Shader::use() const {
    // activate the program and every rendering call onwards will use the program
    glUseProgram(this->id);
}

unsigned int Shader::get_fragment_shader_id() const {
    return this->fragment_shader_id;
}

unsigned int Shader::get_vertex_shader_id() const {
    return this->vertex_shader_id;
}

unsigned int Shader::get_program_id() const {
    return this->id;
}

void Shader::set_uniform(const string &name, float value) {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::set_uniform(const string &name, int value) {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::set_uniform(const string &name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::set_uniform(const string &name, float v1, float v2, float v3) {
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3);
}

void Shader::set_uniform(const string &name, float v1, float v2, float v3, float v4) {
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3, v4);
}


