#pragma once

#include <glimac/glm.hpp>

struct TrackballCamera {
public:
    TrackballCamera()
    : m_fDistance {5.}
    , m_fAngleX {0.}
    , m_fAngleY {0.}
    {}

    void moveFront(float delta) 
    {
        m_fDistance += (-delta);
    }

    void rotateLeft(float degrees) 
    {
        m_fAngleX -= degrees;
    }

    void rotateUp(float degrees) 
    {
        m_fAngleY += degrees;
        if (m_fAngleY > 89.0f) m_fAngleY = 89.0f;
        if (m_fAngleY < -89.0f) m_fAngleY = -89.0f;
    } 

    glm::mat4 getViewMatrix() const 
    {
        auto mvp_mat = glm::translate(glm::mat4(1), glm::vec3(0, 0, -m_fDistance));
        mvp_mat = glm::rotate(mvp_mat, glm::radians(m_fAngleX), glm::vec3(0., 1., 0.));
        mvp_mat = glm::rotate(mvp_mat, glm::radians(m_fAngleY), glm::vec3(1., 0., 0.));
        return mvp_mat;
    }
private:
    float m_fDistance, m_fAngleX, m_fAngleY;
};