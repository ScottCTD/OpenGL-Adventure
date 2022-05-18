cmake_minimum_required(VERSION 3.22)
project(StarryDream)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

add_executable(StarryDream src/main.cpp)
target_link_libraries(StarryDream PUBLIC opengl32 glfw glad)