#ifndef __TD_CAMERAS_FIRSTPERSONCAMERA_H__
#define __TD_CAMERAS_FIRSTPERSONCAMERA_H__

#include <GLFW/glfw3.h>

#include "KeyboardState.h"
#include "MouseState.h"

#include "Camera.h"

#include <glm/gtx/rotate_vector.hpp>

namespace td
{
    namespace cameras
    {
        class FirstPersonCamera : public td::cameras::Camera
        {
            private:
                float m_advanceSpeed, m_strafeSpeed;
                float m_verticalSensitivity, m_horizontalSensitivity;

            protected:
                virtual void internalUpdate()
                {
                    if (m_keyboardState->isKeyPressed(GLFW_KEY_W))
                    {
                        m_position += (m_forward * m_advanceSpeed * m_timeFactor);
                    }

                    if (m_keyboardState->isKeyPressed(GLFW_KEY_S))
                    {
                        m_position += -1.0f * (m_forward * m_advanceSpeed * m_timeFactor);
                    }

                    vec3 leftVector = getLeftVector();
                    if (m_keyboardState->isKeyPressed(GLFW_KEY_A))
                    {
                        m_position += (leftVector * m_strafeSpeed * m_timeFactor);
                    }

                    if (m_keyboardState->isKeyPressed(GLFW_KEY_D))
                    {
                        m_position += -1.0f * (leftVector * m_strafeSpeed * m_timeFactor);
                    }

                    if (m_keyboardState->isKeyPressed(GLFW_KEY_C))
                    {
                        m_position += -1.0f * m_up * m_timeFactor * m_strafeSpeed;
                    }

                    if (m_keyboardState->isKeyPressed(GLFW_KEY_SPACE))
                    {
                        m_position += m_up * m_timeFactor * m_strafeSpeed;
                    }

                    if (m_mouseState->isLeftButtonDown())
                    {
                        float angle = -1.0f * (float)m_mouseState->dx() * m_timeFactor * m_horizontalSensitivity;
                        m_forward = rotate(m_forward, angle, m_up);

                        float yaw = sin(m_forward.y);
                        yaw = abs(degrees(yaw));

                        if (yaw < 70)
                        {
                            angle = -1.0f * (float)m_mouseState->dy() * m_timeFactor * m_verticalSensitivity;
                            m_forward = rotate(m_forward, angle, leftVector);
                        }
                    }
                }

            public:
                FirstPersonCamera(MouseState const * const mouseState, KeyboardState const * const keyboardState)
                    : td::cameras::Camera(mouseState, keyboardState)
                {
                    m_advanceSpeed = 15.0f;
                    m_strafeSpeed = 18.0f;
                    m_verticalSensitivity = 700.0f;
                    m_horizontalSensitivity = 700.0f;
                }

                float & verticalSensitivity() { return m_verticalSensitivity; }
                float & horizontalSensitivity() { return m_horizontalSensitivity; }
                float & advanceSpeed() { return m_advanceSpeed; }
                float & strafeSpeed() { return m_strafeSpeed; }
        };
    }
}

#endif // __TD_CAMERAS_FIRSTPERSONCAMERA_H__
