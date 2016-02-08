#ifndef __TD_GPU_PROGRAMLAYOUT_H__
#define __TD_GPU_PROGRAMLAYOUT_H__

#include <stdio.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "Vertex.h"
#include "shaderBlocks.h"

#include "UBO.h"

using namespace glm;

using namespace td;
using namespace gpu;
using namespace shaderBlocks;

namespace td
{
    namespace gpu
    {
        class ProgramLayout
        {
        private:

            UBO m_viewBlockUBO, m_materialDiffuseBlockUBO, m_materialSpecularBlockUBO, m_pointLightBlockUBO, m_directionalLightBlockUBO;

            ViewBlock m_viewBlock;
            MaterialSpecularBlock m_materialSpecularBlock;
            MaterialDiffuesBlock m_materialDiffuseBlock;
            PointLightBlock m_pointLightBlock;
            DirectionalLightBlock m_directionalLightBlock;

        public:

            struct
            {
                struct
                {
                    GLint model;
                } projections;

                GLint ambient;

            } uniforms;

            struct
            {
                GLint position;
                GLint normal;
            } attributes;

            ProgramLayout() :
                m_viewBlockUBO(1, "ViewBlock"),
                m_materialDiffuseBlockUBO(2, "MaterialDiffuseBlock"),
                m_materialSpecularBlockUBO(3, "MaterialSpecularBlock"),
                m_pointLightBlockUBO(4, "PointLightBlock"),
                m_directionalLightBlockUBO(5, "DirectionalLightBlock")
            { }

            inline ViewBlock & viewBlock(){ return m_viewBlock; }
            inline MaterialSpecularBlock & materialSpecularBlock() { return m_materialSpecularBlock; }
            inline MaterialDiffuesBlock & materialDiffuseBlock() { return m_materialDiffuseBlock; }
            inline PointLightBlock & pointLightBlock() { return m_pointLightBlock; }
            inline DirectionalLightBlock & directionalLightBlock() { return m_directionalLightBlock; }

            void remapViewBlockUBO() { m_viewBlockUBO.remap(); }
            void remapMaterialSpecularBlockUBO() { m_materialSpecularBlockUBO.remap(); }
            void remapMaterialDiffuseBlockUBO() { m_materialDiffuseBlockUBO.remap(); }
            void remapPointLightBlock() { m_pointLightBlockUBO.remap(); }
            void remapDirectionalLightBlock() { m_directionalLightBlockUBO.remap(); }

            inline void setModelProjectionUniform(const mat4 & model)
            {
                glUniformMatrix4fv(uniforms.projections.model, 1, GL_FALSE, value_ptr(model));
            }

            inline void setAmbientUniform(const vec3 & ambient)
            {
                glUniform3fv(uniforms.ambient, 1, value_ptr(ambient));
            }

            inline void enableAllAttributes()
            {
                glEnableVertexAttribArray(attributes.position);
                glEnableVertexAttribArray(attributes.normal);
            }

            inline void disableAllAttributes()
            {
                glDisableVertexAttribArray(attributes.position);
                glDisableVertexAttribArray(attributes.normal);
            }

            void declareVertexAtribPointers()
            {
                glVertexAttribPointer(
                    attributes.position,
                    Vertex::Position.Components,
                    Vertex::Position.Type,
                    Vertex::Position.Normalize,
                    sizeof(Vertex),
                    (void *)Vertex::Position.Offset
                );

                glVertexAttribPointer(
                    attributes.normal,
                    Vertex::Normal.Components,
                    Vertex::Normal.Type,
                    Vertex::Normal.Normalize,
                    sizeof(Vertex),
                    (void *)Vertex::Normal.Offset
                );
            }

            void print()
            {
                printf("Layout locations:\n");
                printf("\tuniforms.projections.model %d\n", uniforms.projections.model);
                printf("\tuniforms.ambient %d\n", uniforms.ambient);

                printf("\tattributes.position %d\n", attributes.position);
                printf("\tattributes.normal %d\n", attributes.normal);
            }

            void acquireLayoutLocations(GLuint program)
            {
                uniforms.projections.model = glGetUniformLocation(program, "model_projection");

                uniforms.ambient = glGetUniformLocation(program, "ambient");

                attributes.position = glGetAttribLocation(program, "in_position");
                attributes.normal = glGetAttribLocation(program, "in_normal");

                // UBOs initialiations
                m_viewBlockUBO.initialize(program, sizeof(m_viewBlock), &m_viewBlock);

                m_materialDiffuseBlockUBO.initialize(program, sizeof(m_materialDiffuseBlock), &m_materialDiffuseBlock);
                m_materialSpecularBlockUBO.initialize(program, sizeof(m_materialSpecularBlock), &m_materialSpecularBlock);

                m_pointLightBlockUBO.initialize(program, sizeof(m_pointLightBlock), &m_pointLightBlock);
                m_directionalLightBlockUBO.initialize(program, sizeof(m_directionalLightBlock), &m_directionalLightBlock);
            }

            void checkError(const char * context)
            {
                int error = glGetError();

                if (error != 0)
                {
                    printf("Error (%s): %d\n", context, error);
                }
            }
        };
    }
}

#endif // __TD_GPU_PROGRAMLAYOUT_H__
