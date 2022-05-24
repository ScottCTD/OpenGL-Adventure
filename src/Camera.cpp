//
// Created by Scott on 2022/5/24.
//

#include "Camera.h"

Camera::Camera(vec3 position_v, vec3 direction_v, vec3 up_v, float yaw_f, float pitch_f) :
        position(position_v), direction(direction_v), world_up(up_v), yaw(yaw_f), pitch(pitch_f) {
    this->update();
}

void Camera::update() {
    glm::vec3 new_direction;
    new_direction.x = cosf(glm::radians(this->yaw)) * cosf(glm::radians(this->pitch));
    new_direction.y = sinf(glm::radians(this->pitch));
    new_direction.z = sinf(glm::radians(this->yaw)) * cosf(glm::radians(this->pitch));
    this->direction = glm::normalize(new_direction);
    this->right = glm::normalize(glm::cross(this->direction, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->direction));
}

mat4 Camera::get_view_matrix() {
    return glm::lookAt(this->position, this->position + this->direction, this->up);
}

void Camera::process_keyboard_input(CameraMovement movement, float delta_time) {
    float speed = delta_time * 6;
    switch (movement) {
        case FORWARD:
            this->position += this->direction * speed;
            break;
        case BACKWARD:
            this->position -= this->direction * speed;
            break;
        case LEFT:
            this->position -= this->right * speed;
            break;
        case RIGHT:
            this->position += this->right * speed;
            break;
        case UP:
            this->position += this->up * speed;
            break;
        case DOWN:
            this->position -= this->up * speed;
            break;
        default:
            break;
    }
}

void Camera::process_mouse_input(float delta_x, float delta_y) {
    this->yaw += delta_x * MOUSE_SENSITIVITY;
    this->pitch += delta_y * MOUSE_SENSITIVITY;

    if (this->pitch > 89.0F) {
        this->pitch = 89.0F;
    } else if (this->pitch < -89.0F) {
        this->pitch = -89.0F;
    }

    this->update();
}
