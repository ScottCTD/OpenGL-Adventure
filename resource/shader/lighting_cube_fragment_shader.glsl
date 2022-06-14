#version 330 core

out vec4 final_color;

in vec3 normal;
in vec3 position;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    // ambient lighting
    float ambient_strength = 0.1;
    vec3 ambient_light = ambient_strength * light_color;

    // diffuse lighting
    vec3 nnormal = normalize(normal);
    vec3 light_direction = normalize(light_position - position);
    float diffuse_factor = max(dot(nnormal, light_direction), 0.0);
    vec3 diffuse_light = diffuse_factor * light_color;

    // specular lighting
    float specular_strength = 0.5;
    vec3 view_direction = normalize(view_position - position);
    // the first vector should point from the light source toward the fragment's position
    vec3 reflect_direction = reflect(-light_direction, nnormal);
    float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0), 64);
    vec3 specular_light = specular_strength * specular_factor * light_color;

    // final result
    vec3 result = (ambient_light + diffuse_light + specular_light) * object_color;
    final_color = vec4(result, 1.0);
}
