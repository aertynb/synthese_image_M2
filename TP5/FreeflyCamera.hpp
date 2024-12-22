#pragma once

#include <glimac/glm.hpp>

namespace glm 
{
    struct FreeflyCamera 
    {
    public:
        FreeflyCamera() 
        : m_fPhi {M_PI}
        , m_fTheta {0.}
        {
            computeDirectionVectors();
        }

        void moveLeft(float t) 
        {
            m_Position += t * m_LeftVector;
        }

        void moveUp(float t) 
        {
            m_Position += t * m_FrontVector; 
        }

        void rotateLeft(float degrees) 
        {
            m_fPhi -= radians(degrees) * 5;
            computeDirectionVectors();
        }

        void rotateUp(float degrees) 
        {
            m_fTheta += radians(degrees) * 5;
            if (m_fTheta > 89.0f) m_fTheta = 89.0f;
            if (m_fTheta < -89.0f) m_fTheta = -89.0f;
            computeDirectionVectors();
        } 


        mat4 getViewMatrix() const 
        {
            return lookAt(m_Position, m_Position + m_FrontVector, normalize(m_UpVector));
        }

    private:
        vec3 m_FrontVector, m_Position, m_LeftVector, m_UpVector;
        float m_fPhi, m_fTheta;

        void computeDirectionVectors() 
        {
            auto radTheta = radians(m_fTheta);
            auto radPhi = radians(m_fPhi);
            m_FrontVector = vec3(cos(radTheta)*sin(radPhi), sin(radTheta), cos(radTheta)*cos(radPhi));
            m_LeftVector = vec3(sin(radPhi+M_PI/2), 0, cos(radPhi+M_PI/2));
            m_UpVector = cross(m_FrontVector, m_LeftVector);
        }
    };
}
