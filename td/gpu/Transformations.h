#ifndef __TD_GPU_TRANSFORMATIONS_H__
#define __TD_GPU_TRANSFORMATIONS_H__

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace td
{
    namespace gpu
    {
        class Transformations
        {
        private:
            glm::mat4 m_translation;
            glm::mat4 m_scale;
            glm::mat4 m_rotation;

        protected:

        public:
            Transformations()
            {
                m_translation = glm::mat4(1.0f);
                m_scale = glm::mat4(1.0f);
                m_rotation = glm::mat4(1.0f);
            }

            /**
                translate
            **/

            Transformations & setTranslation(const float& x, const float& y, const float& z)
            {
                m_translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
                return (*this);
            }

            Transformations & translate(const float& x, const float& y, const float& z)
            {
                m_translation = glm::translate(m_translation, glm::vec3(x, y, z));
                return (*this);
            }

            /**
                scale
            **/

            Transformations & scale(const float& factor)
            {
                m_scale = glm::scale(m_scale, glm::vec3(factor, factor, factor));
                return (*this);
            }

            Transformations & scale(const float& x, const float& y, const float& z)
            {
                m_scale = glm::scale(m_scale, glm::vec3(x, y, z));
                return (*this);
            }

            Transformations & scaleX(const float& factor)
            {
                scale(factor, 1.0f, 1.0f);
                return (*this);
            }

            Transformations & scaleY(const float& factor)
            {
                scale(1.0f, factor, 1.0f);
                return (*this);
            }

            Transformations & scaleZ(const float& factor)
            {
                scale(1.0f, 1.0f, factor);
                return (*this);
            }

            /**
                rotateRadians
            **/

            Transformations & rotateRadians(const float& radians, const float& x, const float& y, const float& z)
            {
                m_rotation *= glm::rotate(radians, glm::normalize(glm::vec3(x, y, z)));
                return (*this);
            }

            Transformations & rotateRadiansAroundX(const float& radians)
            {
                rotateRadians(radians, 1.0f, 0.0f, 0.0f);
                return (*this);
            }

            Transformations & rotateRadiansAroundY(const float& radians)
            {
                rotateRadians(radians, 0.0f, 1.0f, 0.0f);
                return (*this);
            }

            Transformations & rotateRadiansAroundZ(const float& radians)
            {
                rotateRadians(radians, 0.0f, 0.0f, 1.0f);
                return (*this);
            }

            /**
                rotateDegrees
            **/

            Transformations & rotateDegrees(const float& degrees, const float& x, const float& y, const float& z)
            {
                rotateRadians(glm::radians(degrees), x, y, z);
                return (*this);
            }

            Transformations & rotateDegreesAroundX(const float& degrees)
            {
                rotateDegrees(degrees, 1.0f, 0.0f, 0.0f);
                return (*this);
            }

            Transformations & rotateDegreesAroundY(const float& degrees)
            {
                rotateDegrees(degrees, 0.0f, 1.0f, 0.0f);
                return (*this);
            }

            Transformations & rotateDegreesAroundZ(const float& degrees)
            {
                rotateDegrees(degrees, 0.0f, 0.0f, 1.0f);
                return (*this);
            }

            /**
                getModel
            **/

            glm::mat4 getModelMatrix() const
            {
                return m_translation * m_scale * m_rotation;
            }
        };
    }
}

#endif // __TD_GPU_TRANSFORMATIONS_H__
