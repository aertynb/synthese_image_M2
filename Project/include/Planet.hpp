#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/glm.hpp>

struct Planet{
    glm::vec3 position;
    GLuint texture;
    float scale;
    float speed;
    float rotationSpeed;
};