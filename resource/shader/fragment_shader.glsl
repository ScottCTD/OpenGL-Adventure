#version 330 core

out vec4 final_color;

uniform float green_value;

void main()
{
    final_color = vec4(0.0, green_value, 0.0, 1.0);
}