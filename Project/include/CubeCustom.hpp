#pragma once

#include <vector>
#include "glimac/common.hpp"
#include "glad/glad.h"
#include "Object.hpp"
#include "CubeVertex.hpp"

namespace glimac {

// Represents a discretized cube in local space.
// Its default orientation is aligned with the axes, centered at (0, 0, 0) in local coordinates.
// Each face of the cube has texture coordinates and normal vectors.
class CubeCustom : public Object {
public:
    // Constructor: Allocates data and builds the vertex attributes.
    CubeCustom(GLfloat width, GLfloat height, GLfloat depth):
        Object()
        , m_nVertexCount(0) {
        build(width, height, depth); // Build method (implementation in the .cpp)
    }

    // Returns a pointer to the data
    const CubeVertex* getDataPointer() const {
        return &m_Vertices[0];
    }

    // Returns the number of vertices
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

    const unsigned long getVertexSize() const {
        return CubeVertex::sizeOfVertex();
    }

    // Initializes VAO pointers for position, normal, and texture coordinates
    void initVaoPointer(GLuint vPos, GLuint vNorm, GLuint vTex) const {
        std::cout << "cube vao init" << std::endl;
        glEnableVertexAttribArray(vPos);
        glEnableVertexAttribArray(vNorm);
        glEnableVertexAttribArray(vTex);
        glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, getVertexSize(), (GLvoid*) offsetof(glimac::CubeVertex, position));
        glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, getVertexSize(), (GLvoid*) offsetof(glimac::CubeVertex, normal));
        glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, getVertexSize(), (GLvoid*) offsetof(glimac::CubeVertex, texCoords));
    }

private:
    // Builds the cube data
    void build(GLfloat width, GLfloat height, GLfloat depth) {
        GLfloat halfWidth = width / 2.f;
        GLfloat halfHeight = height / 2.f;
        GLfloat halfDepth = depth / 2.f;

        // Define the vertices for a cube (12 triangles forming 6 rectangle faces)
        CubeVertex vertices[] = {
            // Front face
            {{-halfWidth, -halfHeight,  halfDepth}, {0.f, 0.f, 1.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth, -halfHeight,  halfDepth}, {0.f, 0.f, 1.f}, {1.f, 0.f}}, // Bottom-right
            {{ halfWidth,  halfHeight,  halfDepth}, {0.f, 0.f, 1.f}, {1.f, 1.f}}, // Top-right

            {{-halfWidth, -halfHeight,  halfDepth}, {0.f, 0.f, 1.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth,  halfHeight,  halfDepth}, {0.f, 0.f, 1.f}, {1.f, 1.f}}, // Top-right
            {{-halfWidth,  halfHeight,  halfDepth}, {0.f, 0.f, 1.f}, {0.f, 1.f}}, // Top-left

            // Back face
            {{ halfWidth, -halfHeight, -halfDepth}, {0.f, 0.f, -1.f}, {0.f, 0.f}}, // Bottom-left
            {{-halfWidth, -halfHeight, -halfDepth}, {0.f, 0.f, -1.f}, {1.f, 0.f}}, // Bottom-right
            {{-halfWidth,  halfHeight, -halfDepth}, {0.f, 0.f, -1.f}, {1.f, 1.f}}, // Top-right

            {{ halfWidth, -halfHeight, -halfDepth}, {0.f, 0.f, -1.f}, {0.f, 0.f}}, // Bottom-left
            {{-halfWidth,  halfHeight, -halfDepth}, {0.f, 0.f, -1.f}, {1.f, 1.f}}, // Top-right
            {{ halfWidth,  halfHeight, -halfDepth}, {0.f, 0.f, -1.f}, {0.f, 1.f}}, // Top-left

            // Left face
            {{-halfWidth, -halfHeight, -halfDepth}, {-1.f, 0.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{-halfWidth, -halfHeight,  halfDepth}, {-1.f, 0.f, 0.f}, {1.f, 0.f}}, // Bottom-right
            {{-halfWidth,  halfHeight,  halfDepth}, {-1.f, 0.f, 0.f}, {1.f, 1.f}}, // Top-right

            {{-halfWidth, -halfHeight, -halfDepth}, {-1.f, 0.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{-halfWidth,  halfHeight,  halfDepth}, {-1.f, 0.f, 0.f}, {1.f, 1.f}}, // Top-right
            {{-halfWidth,  halfHeight, -halfDepth}, {-1.f, 0.f, 0.f}, {0.f, 1.f}}, // Top-left

            // Right face
            {{ halfWidth, -halfHeight,  halfDepth}, {1.f, 0.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth, -halfHeight, -halfDepth}, {1.f, 0.f, 0.f}, {1.f, 0.f}}, // Bottom-right
            {{ halfWidth,  halfHeight, -halfDepth}, {1.f, 0.f, 0.f}, {1.f, 1.f}}, // Top-right

            {{ halfWidth, -halfHeight,  halfDepth}, {1.f, 0.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth,  halfHeight, -halfDepth}, {1.f, 0.f, 0.f}, {1.f, 1.f}}, // Top-right
            {{ halfWidth,  halfHeight,  halfDepth}, {1.f, 0.f, 0.f}, {0.f, 1.f}}, // Top-left

            // Top face
            {{-halfWidth,  halfHeight,  halfDepth}, {0.f, 1.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth,  halfHeight,  halfDepth}, {0.f, 1.f, 0.f}, {1.f, 0.f}}, // Bottom-right
            {{ halfWidth,  halfHeight, -halfDepth}, {0.f, 1.f, 0.f}, {1.f, 1.f}}, // Top-right

            {{-halfWidth,  halfHeight,  halfDepth}, {0.f, 1.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth,  halfHeight, -halfDepth}, {0.f, 1.f, 0.f}, {1.f, 1.f}}, // Top-right
            {{-halfWidth,  halfHeight, -halfDepth}, {0.f, 1.f, 0.f}, {0.f, 1.f}}, // Top-left

            // Bottom face
            {{-halfWidth, -halfHeight, -halfDepth}, {0.f, -1.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth, -halfHeight, -halfDepth}, {0.f, -1.f, 0.f}, {1.f, 0.f}}, // Bottom-right
            {{ halfWidth, -halfHeight,  halfDepth}, {0.f, -1.f, 0.f}, {1.f, 1.f}}, // Top-right

            {{-halfWidth, -halfHeight, -halfDepth}, {0.f, -1.f, 0.f}, {0.f, 0.f}}, // Bottom-left
            {{ halfWidth, -halfHeight,  halfDepth}, {0.f, -1.f, 0.f}, {1.f, 1.f}}, // Top-right
            {{-halfWidth, -halfHeight,  halfDepth}, {0.f, -1.f, 0.f}, {0.f, 1.f}}  // Top-left
        };

        m_Vertices.assign(vertices, vertices + 36); // Add all vertices to the vector
        m_nVertexCount = 36; // Six faces * 2 triangles per face * 3 vertices per triangle
    }

    std::vector<CubeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Number of vertices
};

}
