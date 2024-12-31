#pragma once

#include "ObjectVertex.hpp"
#include "glimac/common.hpp"
#include "glad/glad.h"

#include <vector>

class Object {
public:
    Object() = default;
    virtual ~Object() = default;
    virtual const ObjectVertex* getDataPointer() const = 0;
    // Renvoit le nombre de vertex
    virtual GLsizei getVertexCount() const = 0;
    virtual const unsigned long getVertexSize() const = 0;
    virtual void initVaoPointer(GLuint vPos, GLuint vNorm, GLuint vTex) const = 0;
private:
    std::vector<ObjectVertex> _vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
};