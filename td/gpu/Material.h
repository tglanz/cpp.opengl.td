#ifndef __TD_GPU_MATERIAL_H__
#define __TD_GPU_MATERIAL_H__

#include <glm/glm.hpp>

#include "ProgramLayout.h"

namespace td
{
    namespace gpu
    {
        class Material
        {
        private:
            ProgramLayout * const m_programLayout;

            float m_specularShininess;
            glm::vec3 m_specularColor;


        public:

            Material(ProgramLayout * const programLayout) :
                m_programLayout(programLayout)
            {
                m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
                m_specularShininess = .1f;
            }

            inline ProgramLayout * const programLayout() { return m_programLayout; }

            inline glm::vec3 & specularColor() { return m_specularColor; }
            inline float & specularShininess() { return m_specularShininess; }

            void assignUniforms()
            {
                m_programLayout->setSpecularShininessUniform(m_specularShininess);
                m_programLayout->setSpecularColorUniform(m_specularColor);
            }
        };
    }
}

#endif // __TD_GPU_MATERIAL_H__
