cmake_minimum_required(VERSION 3.22)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

add_subdirectory(lib)
add_executable(LearnOpenGL
        src/main.cpp
        src/Shader.cpp src/Shader.h
        src/Texture2D.cpp src/Texture2D.h
)
target_link_libraries(LearnOpenGL PUBLIC opengl32 glfw glad stb glm)

# run setup.py before building
add_custom_target(
        setup ALL
        COMMAND python setup.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Setting up resources..."
)
add_dependencies(LearnOpenGL setup)
