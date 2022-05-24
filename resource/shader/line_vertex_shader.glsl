#version 330 core

layout (location = 0) in vec3 in_pos;

out vec4 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(in_pos, 1.0);
    if (in_pos.x != 0) {
        color = vec4(1.0, 0.0, 0.0, 1.0);
    } else if (in_pos.y != 0) {
        color = vec4(0.0, 1.0, 0.0, 1.0);
    } else if (in_pos.z != 0) {
        color = vec4(0.0, 0.0, 1.0, 1.0);
    }
}