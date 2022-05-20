#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

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

    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

    // create a vertex array object to store our VBOs
    unsigned vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // vertex data and VBO
    float vertices[] = {
            0.0, 0.5, 0.0, // TOP
            -0.5, -0.5, 0.0, // LEFT
            0.5, -0.5, 0.0 // RIGHT
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // the render loop
    while (!glfwWindowShouldClose(window)) {
        // process inputs
        process_inputs(window);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(vao_id);
        // primitive, the starting index of the vertex array, number of vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap the double buffer
        glfwSwapBuffers(window);
        // process events like keyboard and window updates callbacks
        glfwPollEvents();
    }

    glfwTerminate();
}
