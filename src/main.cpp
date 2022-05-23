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

using std::string;

// a callback function that is called everytime the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_inputs(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
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

    // init glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    // tell OpenGL the size of our rendering window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // register the change window size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
            0.5, -0.5, 0.5, 1.0, 0.0, // right bottom

            // back
            -0.5, -0.5, -0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, -0.5, 0.0, 1.0, // left top
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            0.5, -0.5, -0.5, 1.0, 0.0, // right bottom

            // left
            -0.5, -0.5, -0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, -0.5, 0.0, 1.0, // left top
            -0.5, 0.5, 0.5, 1.0, 1.0, // right top
            -0.5, -0.5, 0.5, 1.0, 0.0, // right bottom

            // right
            0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            0.5, 0.5, 0.5, 0.0, 1.0, // left top
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            0.5, -0.5, -0.5, 1.0, 0.0, // right bottom

            // top
            -0.5, 0.5, 0.5, 0.0, 0.0, // left bottom
            -0.5, 0.5, -0.5, 0.0, 1.0, // left top
            0.5, 0.5, -0.5, 1.0, 1.0, // right top
            0.5, 0.5, 0.5, 1.0, 0.0, // right bottom

            // bottom
            -0.5, -0.5, 0.5, 0.0, 0.0, // left bottom
            -0.5, -0.5, -0.5, 0.0, 1.0, // left top
            0.5, -0.5, -0.5, 1.0, 1.0, // right top
            0.5, -0.5, 0.5, 1.0, 0.0, // right bottom
    };
    unsigned indices[] = {
            // front
            0, 1, 2,
            0, 2, 3,

            // back
            4, 5, 6,
            4, 6, 7,

            // left
            8, 9, 10,
            8, 10, 11,

            // right
            12, 13, 14,
            12, 14, 15,

            // top
            16, 17, 18,
            16, 18, 19,

            // botom
            20, 21, 22,
            20, 22, 23,
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

    unsigned ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    shader.use();
    shader.set_uniform("texture0", 0);
    shader.set_uniform("texture1", 1);

    glm::mat4 model_matrix(1.0), view_matrix(1.0), projection_matrix(
            glm::perspective(glm::radians(45.0F), static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT,
                             0.1F, 100.0F));
    model_matrix = glm::rotate(model_matrix, glm::radians(-30.0F), glm::vec3(1.0, 0.0, 0.0));
    view_matrix = glm::translate(view_matrix, glm::vec3(0.0F, 0.0F, -3.0F));

    shader.set_uniform("model_matrix", model_matrix);
    shader.set_uniform("view_matrix", view_matrix);
    shader.set_uniform("projection_matrix", projection_matrix);

    glEnable(GL_DEPTH_TEST);
    // the render loop
    while (!glfwWindowShouldClose(window)) {
        // process inputs
        process_inputs(window);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();;
        texture1.bind(GL_TEXTURE0);
        texture2.bind(GL_TEXTURE1);
        model_matrix = glm::rotate(model_matrix, glm::radians(-0.5F), glm::vec3(0.0, 1.0, 0.0));
        shader.set_uniform("model_matrix", model_matrix);
        glBindVertexArray(vao_id);
        // primitive, the starting index of the vertex array, number of vertices
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // swap the double buffer
        glfwSwapBuffers(window);
        // process events like keyboard and window updates callbacks
        glfwPollEvents();
    }

    glfwTerminate();
}
