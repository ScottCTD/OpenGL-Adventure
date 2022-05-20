cmake_minimum_required(VERSION 3.22)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

add_subdirectory(lib)

add_executable(LearnOpenGL src/main.cpp src/Shader.cpp src/Shader.h)
target_link_libraries(LearnOpenGL PUBLIC opengl32 glfw glad)

configure_file(resource/shader/vertex_shader.glsl build/vertex_shader.glsl COPYONLY)
configure_file(resource/shader/fragment_shader.glsl build/fragment_shader.glsl COPYONLY)