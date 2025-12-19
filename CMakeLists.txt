cmake_minimum_required(VERSION 3.16)
project(rocket-simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/window.cpp
    src/shader.cpp
    src/mesh.cpp
    src/renderer.cpp
    src/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/header
)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    glm::glm
    assimp
)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR})