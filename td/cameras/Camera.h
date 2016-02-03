#ifndef __TD_CAMERAS_CAMERA_H__
#define __TD_CAMERAS_CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MouseState.h"
#include "KeyboardState.h"

using namespace glm;

namespace td
{
    namespace cameras
    {
        class Camera
        {
        private:
            mat4 m_viewMatrix;

            void updateViewMatrix()
            {
                m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
            }

        protected:

            MouseState const * m_mouseState;
            KeyboardState const * m_keyboardState;

            vec3 m_up;
            vec3 m_forward;
            vec3 m_position;
            float m_timeFactor;

            virtual void internalUpdate()
            {
                // Nothing to see here
            }

        public:
            Camera(MouseState const * const mouseState, KeyboardState const * const keyboardState)
            {
                m_mouseState = mouseState;
                m_keyboardState = keyboardState;

                m_position = vec3(0, 0, -5);
                m_forward = vec3(0, 0, 1);
                m_up = vec3(0, 1, 0);

                m_timeFactor = 1.0f;
            }

            float & timeFactor() { return m_timeFactor; }

            mat4 const & getViewMatrix() const { return m_viewMatrix; }

            vec3 getLeftVector() const
            {
                return normalize(cross(m_up, m_forward));
            }

            void lookAtTarget(vec3 const & target)
            {
                m_forward = normalize(target - m_position);
            }

            void setPosition(vec3 const & position)
            {
                m_position = position;
            }

            const glm::vec3 & getPosition()
            {
                return m_position;
            }

            void setUp(vec3 const & up)
            {
                m_up = normalize(up);
            }

            void update()
            {
                internalUpdate();

                updateViewMatrix();
            }
        };
    }
}

#endif // __TD_CAMERAS_CAMERA_H__
