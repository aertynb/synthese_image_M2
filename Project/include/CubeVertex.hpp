#pragma once

#include <glimac/glm.hpp>
#include "ObjectVertex.hpp"

namespace glimac {
    // Represents a single vertex of a cube
    struct CubeVertex : public ObjectVertex {

        CubeVertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& tex) :
        position {pos}, normal {norm}, texCoords {tex}
        {}

        static unsigned long sizeOfVertex() {
           return sizeof(position) + sizeof(normal) + sizeof(texCoords);
        }

        glm::vec3 position;  // Vertex position
        glm::vec3 normal;    // Normal vector
        glm::vec2 texCoords; // Texture coordinates
    };

}