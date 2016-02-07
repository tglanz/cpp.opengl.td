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

            glm::vec3 m_diffuseColor;

        public:

            Material(ProgramLayout * const programLayout) :
                m_programLayout(programLayout)
            {
                m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
                m_specularShininess = .1f;
            }

            ProgramLayout * const programLayout() { return m_programLayout; }

            inline glm::vec3 & specularColor() { return m_specularColor; }
            inline float & specularShininess() { return m_specularShininess; }
            inline glm::vec3 & diffuseColor() { return m_diffuseColor; }

            void assignUniforms()
            {
                m_programLayout->materialSpecularBlock().color = m_specularColor;
                m_programLayout->materialSpecularBlock().shininess = m_specularShininess;
                m_programLayout->materialDiffuseBlock().color = m_diffuseColor;

                m_programLayout->remapMaterialSpecularBlockUBO();
                m_programLayout->remapMaterialDiffuseBlockUBO();
            }
        };
    }
}

#endif // __TD_GPU_MATERIAL_H__
