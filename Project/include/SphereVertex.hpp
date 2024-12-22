#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <glimac/glm.hpp>
#include "ObjectVertex.hpp"

namespace glimac {

struct SphereVertex : public ObjectVertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

public:
    static unsigned long sizeOfVertex() {
        return sizeof(position) + sizeof(normal) + sizeof(texCoords);
    }
};

}