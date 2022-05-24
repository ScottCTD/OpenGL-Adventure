#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


using std::string;

// camera
Camera camera;

// timing
float delta_time = 0;
float last_frame_time = 0;

// mouse
bool first_move = true;
float last_x = WINDOW_WIDTH / 2.0F, last_y = WINDOW_HEIGHT / 2.0F;

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
    float delta_x = (x_pos - last_x);
    float delta_y = (last_y - y_pos);
    last_x = x_pos;
    last_y = y_pos;

    camera.process_mouse_input(delta_x, delta_y);
}

void process_inputs(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.process_keyboard_input(CameraMovement::FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.process_keyboard_input(CameraMovement::BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.process_keyboard_input(CameraMovement::LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.process_keyboard_input(CameraMovement::RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.process_keyboard_input(CameraMovement::UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.process_keyboard_input(CameraMovement::DOWN, delta_time);
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

unsigned int init_cube_vao() {
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
    return vao_id;
}

unsigned int init_coordinates_vao() {
    float line_vertices[] = {
        // x-axis
        -1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        // y-axis
        0.0, -1.0, 0.0,
        0.0, 1.0, 0.0,
        // z-axis
        0.0, 0.0, -1.0,
        0.0, 0.0, 1.0,
    };
    unsigned line_vao_id;
    glGenVertexArrays(1, &line_vao_id);
    glBindVertexArray(line_vao_id);
    unsigned line_vbo_id;
    glGenBuffers(1, &line_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return line_vao_id;
}

unsigned int init_crosshair_vao() {
    float vertices[] = {
            -0.02, 0.0, 0.0,
            0.02, 0.0, 0.0,
            0.0, -0.02, 0.0,
            0.0, 0.02, 0.0
    };
    unsigned vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    unsigned vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return vao;
}

int main() {

    auto *window = initialize();

    // matrices
    glm::mat4 projection_matrix(glm::perspective(glm::radians(45.0F),
                                                 static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT,
                                                 0.1F, 100.0F));
    glm::mat4 view_matrix = camera.get_view_matrix();

    // crosshair
    Shader crosshair_shader("resource/shader/crosshair_vertex_shader.glsl",
                            "resource/shader/crosshair_fragment_shader.glsl");
    unsigned crosshair_vao = init_crosshair_vao();

    // cube initialization
    Texture2D cube_texture1("resource/texture/grass_block_side.png");
    Texture2D cube_texture2("resource/texture/oak_planks.png");
    cube_texture1.bind(GL_TEXTURE0);
    cube_texture2.bind(GL_TEXTURE1);
    Shader cube_shader("resource/shader/cube_vertex_shader.glsl",
                       "resource/shader/cube_fragment_shader.glsl");
    cube_shader.use();
    cube_shader.set_uniform("texture0", 0);
    cube_shader.set_uniform("texture1", 1);
    glm::mat4 cube_model_matrix = glm::translate(glm::mat4(1.0F), glm::vec3(0.5F));
    cube_shader.set_uniform("model_matrix", cube_model_matrix);
    cube_shader.set_uniform("view_matrix", view_matrix);
    cube_shader.set_uniform("projection_matrix", projection_matrix);
    unsigned cube_vao = init_cube_vao();

    // coordinate line
    Shader coordinate_shader("resource/shader/line_vertex_shader.glsl",
                             "resource/shader/line_fragment_shader.glsl");
    glm::mat4 line_model_matrix = glm::scale(glm::mat4(1.0F), glm::vec3(10000.0F));
    coordinate_shader.use();
    coordinate_shader.set_uniform("model_matrix", line_model_matrix);
    coordinate_shader.set_uniform("view_matrix", view_matrix);
    coordinate_shader.set_uniform("projection_matrix", projection_matrix);
    unsigned coordinate_vao = init_coordinates_vao();

    glEnable(GL_DEPTH_TEST);
    // the render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        // reset color and depth information
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // timing
        auto current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_frame_time;
        last_frame_time = current_time;
        std::cout << "FPS: " << 1 / delta_time << std::endl;
        // process inputs
        process_inputs(window);
        // update view matrix
        view_matrix = camera.get_view_matrix();

        // crosshair
        crosshair_shader.use();
        glBindVertexArray(crosshair_vao);
        glDrawArrays(GL_LINES, 0, 4);

        // render cube
        cube_shader.use();;
        cube_shader.set_uniform("view_matrix", view_matrix);
        for (int i = 0; i != 16; ++i) {
            for (int j = 0; j != 16; ++j) {
                auto temp_model_matrix = glm::translate(cube_model_matrix, glm::vec3(i, 0, j));
                cube_shader.set_uniform("model_matrix", temp_model_matrix);
                glBindVertexArray(cube_vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        // render coordinate
        coordinate_shader.use();
        coordinate_shader.set_uniform("view_matrix", view_matrix);
        glBindVertexArray(coordinate_vao);
        glDrawArrays(GL_LINES, 0, 6);

        // swap the double buffer
        glfwSwapBuffers(window);
        // process events like keyboard and window updates callbacks
        glfwPollEvents();
    }

    glfwTerminate();
}
