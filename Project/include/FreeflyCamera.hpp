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

        void tryMoveLeft(float t) {
            glm::vec3 newPosition = m_Position + t * m_LeftVector;
            if (!checkCollision(newPosition)) {
                m_Position = newPosition;
            }
        }

        void tryMoveUp(float t) {
            glm::vec3 horizontalFront = glm::vec3(m_FrontVector.x, 0, m_FrontVector.z);
            glm::vec3 newPosition = m_Position + t * glm::normalize(horizontalFront);
            if (!checkCollision(newPosition)) {
                m_Position = newPosition;
            }
        }

        void rotateLeft(float degrees) {
            m_fPhi -= glm::radians(degrees) * 5;
            computeDirectionVectors();
        }

        void rotateUp(float degrees) {
            m_fTheta += glm::radians(degrees) * 5;
            if (m_fTheta > 89.0f) m_fTheta = 89.0f;
            if (m_fTheta < -89.0f) m_fTheta = -89.0f;
            computeDirectionVectors();
        } 

        glm::mat4 getViewMatrix() const {
            return glm::lookAt(m_Position, m_Position + m_FrontVector, glm::normalize(m_UpVector));
        }

        bool isInFirstRoom() const {
            return m_Position.z >= 0;
        }

    private:
        glm::vec3 m_FrontVector, m_Position, m_LeftVector, m_UpVector;
        float m_fPhi, m_fTheta;

        void computeDirectionVectors() {
            auto radTheta = glm::radians(m_fTheta);
            auto radPhi = glm::radians(m_fPhi);
            m_FrontVector = glm::vec3(cos(radTheta)*sin(radPhi), sin(radTheta), cos(radTheta)*cos(radPhi));
            m_LeftVector = glm::vec3(sin(radPhi+M_PI/2), 0, cos(radPhi+M_PI/2));
            m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
        }

        bool checkCollision(const glm::vec3& position) {
            if (position.x < -12 || position.x > 12) {
                return true;
            }
            if (position.z < -21 || position.z > 21) {
                return true;
            }
            if (position.z > -1 && position.z < 1) {
                if (position.x < -2 || position.x > 2) {
                    return true;
                }
            }
            return false;
        }
    };
}
