#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture2D.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define SENSITIVITY 0.1F

using std::string;

// camera
glm::vec3 camera_pos(0.0F, 0.0F, 6.0F);
glm::vec3 camera_direction(0.0F, 0.0F, -1.0F);
glm::vec3 camera_up(0.0F, 1.0F, 0.0F);

// timing
float delta_time = 0;
float last_frame_time = 0;

// mouse
bool first_move = true;
float last_x = WINDOW_WIDTH / 2.0F, last_y = WINDOW_HEIGHT / 2.0F;
float yaw = -90.0F, pitch = 0.0F;

// a callback function that is called everytime the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double dx_pos, double dy_pos) {
    auto x_pos = static_cast<float>(dx_pos);
    auto y_pos = static_cast<float>(dy_pos);
    if (first_move) {
        last_x = x_pos;
        last_y = y_pos;
        first_move = false;
    }
    float delta_x = (x_pos - last_x) * SENSITIVITY;
    float delta_y = (last_y - y_pos) * SENSITIVITY;
    last_x = x_pos;
    last_y = y_pos;

    yaw += delta_x;
    pitch += delta_y;

    if (pitch > 89.0F) {
        pitch = 89.0F;
    } else if (pitch < -89.0F) {
        pitch = -89.0F;
    }

    glm::vec3 new_direction;
    new_direction.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    new_direction.y = sinf(glm::radians(pitch));
    new_direction.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    camera_direction = glm::normalize(new_direction);
}

void process_inputs(GLFWwindow *window) {
    float speed = delta_time * 5.0F;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_pos += camera_direction * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_pos -= camera_direction * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_pos -= glm::normalize(glm::cross(camera_direction, camera_up)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_pos += glm::normalize(glm::cross(camera_direction, camera_up)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera_pos += camera_up * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera_pos -= camera_up * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        int mode;
        glGetIntegerv(GL_POLYGON_MODE, &mode);
        if (mode == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

GLFWwindow *initialize() {
    std::cout << "Hello OpenGL!" << std::endl;

    // init GLFW
    glfwInit();
    // tell GLFW that we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // tell GLFW we need core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create the window object
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFWwindow object!");
    }

    // make the context of our window the main context on the current thread
    glfwMakeContextCurrent(window);
    // make the cursor invisible and cannot leave the window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    // tell OpenGL the size of our rendering window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // register the change window size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // register the mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);

    return window;
}

int main() {

    auto *window = initialize();

    // texture
    Texture2D texture1("resource/texture/grass_block_side.png");
    Texture2D texture2("resource/texture/oak_planks.png");

    Shader shader("resource/shader/vertex_shader.glsl",
                  "resource/shader/fragment_shader.glsl");

    // create a vertex array object to store our VBOs
    unsigned vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // vertex data and VBO
    float vertices[] = {
            // 3 positions,                2 tex coords
            // front
            -0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, 0.5, 0.0, 1.0, // left top
            0.5, 0.5, 0.5, 1.0, 1.0, // right top
            -0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            0.5, 0.5, 0.5, 1.0, 1.0, // right top
            0.5, -0.5, 0.5, 1.0, 0.0, // right bottom
            // back
            -0.5, -0.5, -0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, -0.5, 0.0, 1.0, // left top
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            -0.5, -0.5, -0.5, 0.0, 0.0, // left bottom
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            0.5, -0.5, -0.5, 1.0, 0.0, // right bottom
            // left
            -0.5, -0.5, -0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, -0.5, 0.0, 1.0, // left top
            -0.5, 0.5, 0.5, 1.0, 1.0, // right top
            -0.5, -0.5, -0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, 0.5, 1.0, 1.0, // right top
            -0.5, -0.5, 0.5, 1.0, 0.0, // right bottom
            // right
            0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            0.5, 0.5, 0.5, 0.0, 1.0, // left top
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            0.5, -0.5, -0.5, 1.0, 0.0, // right bottom
            // top
            -0.5, 0.5, 0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, -0.5, 0.0, 1.0, // left top
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            -0.5, 0.5, 0.5, 0.0, 0.0, // left bottom
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            0.5, 0.5, 0.5, 1.0, 0.0, // right bottom
            // bottom
            -0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            -0.5, -0.5, -0.5, 0.0, 1.0, // left top
            0.5, -0.5, -0.5, 1.0, 1.0, // right top
            -0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            0.5, -0.5, -0.5, 1.0, 1.0, // right top
            0.5, -0.5, 0.5, 1.0, 0.0, // right bottom
    };
    // our first vertex buffer object
    unsigned vbo_id;
    glGenBuffers(1, &vbo_id);
    // OpenGL has many buffer types, and VBO has GL_ARRAY_BUFFER as its type
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    // copy the previously defined vertex data to the buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // right now, we have the data and a program to process the data, we still need a way to
    // pass the data into the program
    // location, size of vertex attributes, types of the data, normalize?, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    shader.use();
    shader.set_uniform("texture0", 0);
    shader.set_uniform("texture1", 1);

    glm::mat4 projection_matrix(glm::perspective(glm::radians(45.0F), static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT,
                             0.1F, 100.0F));
    glm::mat4 model_matrix = glm::rotate(glm::mat4(1.0), glm::radians(-30.0F), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 view_matrix = glm::lookAt(camera_pos, camera_pos + camera_direction, camera_up);


    shader.set_uniform("model_matrix", model_matrix);
    shader.set_uniform("view_matrix", view_matrix);
    shader.set_uniform("projection_matrix", projection_matrix);

    glEnable(GL_DEPTH_TEST);
    // the render loop
    while (!glfwWindowShouldClose(window)) {
        // timing
        auto current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_frame_time;
        last_frame_time = current_time;
//        std::cout << "FPS: " << 1 / delta_time << std::endl;

        // process inputs
        process_inputs(window);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();;
        texture1.bind(GL_TEXTURE0);
        texture2.bind(GL_TEXTURE1);
        view_matrix = glm::lookAt(camera_pos, camera_pos + camera_direction, camera_up);
        shader.set_uniform("view_matrix", view_matrix);
        glBindVertexArray(vao_id);

        for (int i = 0; i != 10; ++i) {
            model_matrix = glm::translate(glm::mat4(1.0F), glm::vec3(1.0F, 1.0F * i, 1.0F));
            shader.set_uniform("model_matrix", model_matrix);
            // primitive, the starting index of the vertex array, number of vertices
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        model_matrix = glm::rotate(model_matrix, glm::radians(-0.5F), glm::vec3(0.0, 1.0, 0.0));
        shader.set_uniform("model_matrix", model_matrix);

        // swap the double buffer
        glfwSwapBuffers(window);
        // process events like keyboard and window updates callbacks
        glfwPollEvents();
    }

    glfwTerminate();
}
