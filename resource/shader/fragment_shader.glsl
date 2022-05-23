#version 330 core

in vec2 tex_coords;

out vec4 final_color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    final_color = mix(texture(texture0, tex_coords), texture(texture1, tex_coords), 1.0);
}