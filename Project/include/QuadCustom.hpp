#pragma once

#include <vector>
#include "glimac/common.hpp"
#include "glad/glad.h"
#include "Object.hpp"
#include "QuadVertex.hpp"

namespace glimac {

// Represents a discretized quad (rectangle) in local space.
// Its default plane lies on the XZ plane, centered at (0, 0, 0) in local coordinates.
// The quad has texture coordinates and normal vectors.
class QuadCustom : public Object {
public:
    // Constructor: Allocates data and builds the vertex attributes.
    QuadCustom(GLfloat width, GLfloat height):
        Object()
        , m_nVertexCount(0) {
        build(width, height); // Build method (implementation in the .cpp)
    }

    // Returns a pointer to the data
    const QuadVertex* getDataPointer() const {
        return &m_Vertices[0];
    }

    // Returns the number of vertices
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

    const unsigned long getVertexSize() const {
        return QuadVertex::sizeOfVertex();
    }

    // Initializes VAO pointers for position, normal, and texture coordinates
    void initVaoPointer(GLuint vPos, GLuint vNorm, GLuint vTex) const {
        std::cout << "quad vao init" << std::endl;
        glEnableVertexAttribArray(vPos);
        glEnableVertexAttribArray(vNorm);
        glEnableVertexAttribArray(vTex);
        glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, getVertexSize(), (GLvoid*) offsetof(glimac::QuadVertex, position));
        glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, getVertexSize(), (GLvoid*) offsetof(glimac::QuadVertex, normal));
        glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, getVertexSize(), (GLvoid*) offsetof(glimac::QuadVertex, texCoords));
    }

private:
    // Builds the quad data
    void build(GLfloat width, GLfloat height) {
        GLfloat halfWidth = width / 2.f;
        GLfloat halfHeight = height / 2.f;

    // Define the vertices for a quad (two triangles forming a rectangle)
    QuadVertex vertices[] = {
        {{-halfWidth, 0.f, -halfHeight}, {0.f, 1.f, 0.f}, {0.f, 0.f}}, // Bottom-left
        {{ halfWidth, 0.f, -halfHeight}, {0.f, 1.f, 0.f}, {1.f, 0.f}}, // Bottom-right
        {{ halfWidth, 0.f,  halfHeight}, {0.f, 1.f, 0.f}, {1.f, 1.f}}, // Top-right

        {{-halfWidth, 0.f, -halfHeight}, {0.f, 1.f, 0.f}, {0.f, 0.f}}, // Bottom-left
        {{ halfWidth, 0.f,  halfHeight}, {0.f, 1.f, 0.f}, {1.f, 1.f}}, // Top-right
        {{-halfWidth, 0.f,  halfHeight}, {0.f, 1.f, 0.f}, {0.f, 1.f}}  // Top-left
    };

    m_Vertices.assign(vertices, vertices + 6); // Add all vertices to the vector
    m_nVertexCount = 6; // Two triangles (6 vertices total)
    }

    std::vector<QuadVertex> m_Vertices;
    GLsizei m_nVertexCount; // Number of vertices
};

}
