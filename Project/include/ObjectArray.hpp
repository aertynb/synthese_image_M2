#pragma once

#include "Object.hpp"
#include "ObjectVertex.hpp"
#include "glimac/common.hpp"

#include <vector>

class ObjectArray {
public:
    ObjectArray() {};
private :
    std::vector<Object> _objects;
};