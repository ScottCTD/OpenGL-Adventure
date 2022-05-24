//
// Created by Scott on 2022/5/24.
//

#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MOUSE_SENSITIVITY 0.1F

using glm::vec3;
using glm::mat4;

enum CameraMovement {
    FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
};

class Camera {

public:
    vec3 position;
    vec3 direction;
    vec3 right;
    vec3 up;
    vec3 world_up;
    float mouse_x;
    float mouse_y;
    float yaw;
    float pitch;

    explicit
    Camera(vec3 position_v = vec3(0.0F, 0.0F, 0.0F),
           vec3 direction_v = vec3(0.0F, 0.0F, -1.0F),
           vec3 up_v = vec3(0.0F, 1.0F, 0.0F),
           float yaw_f = -90.0F, float pitch_f = 0.0F);

    mat4 get_view_matrix();
    void process_keyboard_input(CameraMovement movement, float delta_time);
    void process_mouse_input(float delta_x, float delta_y);

private:
    void update();
};


#endif //LEARNOPENGL_CAMERA_H
