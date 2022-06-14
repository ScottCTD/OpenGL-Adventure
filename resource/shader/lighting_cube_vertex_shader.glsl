#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

out vec3 normal;
out vec3 position;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat3 normal_matrix;

void main()
{
    // transform the normal vector to the world space
    normal = normal_matrix * in_normal;
    position = vec3(model_matrix * vec4(in_pos, 1.0));
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(in_pos, 1.0);
}