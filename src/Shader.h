//
// Created by Scott on 2022/5/19.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using std::ifstream;
using std::ostringstream;

class Shader {

private:
    unsigned vertex_shader_id;
    unsigned fragment_shader_id;
    unsigned id;

    void init_shader(unsigned shader_id, const string &source) const;

public:
    Shader(const string &vertex_shader_path, const string &fragment_shader_path);
    void use() const;

    unsigned int get_vertex_shader_id() const;
    unsigned int get_program_id() const;
    unsigned int get_fragment_shader_id() const;

    void set_uniform(const string &name, float value) const;
    void set_uniform(const string &name, int value) const;
    void set_uniform(const string &name, unsigned int value) const;
    void set_uniform(const string &name, float v1, float v2, float v3) const;
    void set_uniform(const string &name, float v1, float v2, float v3, float v4) const;
    void set_uniform(const string &name, const glm::mat4 &value, GLboolean transpose = false) const;
};


#endif //LEARNOPENGL_SHADER_H
