#ifndef __TD_GPU_PROGRAMLAYOUT_H__
#define __TD_GPU_PROGRAMLAYOUT_H__

#include <stdio.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"

using namespace glm;

namespace td
{
    namespace gpu
    {
        class ProgramLayout
        {
        private:

        public:
            struct
            {
                struct
                {
                    GLint model;
                    GLint view;
                    GLint perspective;
                } projections;

                GLint eyePosition, ambient;

                struct
                {
                    GLint specularColor;
                    GLint specularShininess;
                } material;

            } uniforms;

            struct
            {
                GLint position;
                GLint color;
                GLint normal;
            } attributes;

            ProgramLayout()
            {

            }

            inline void setViewPerspectiveProjectionsUniforms(const mat4 & view, const mat4 & perspective)
            {
                glUniformMatrix4fv(uniforms.projections.view, 1, GL_FALSE, value_ptr(view));
                glUniformMatrix4fv(uniforms.projections.perspective, 1, GL_FALSE, value_ptr(perspective));
            }

            inline void setModelProjectionUniform(const mat4 & model)
            {
                glUniformMatrix4fv(uniforms.projections.model, 1, GL_FALSE, value_ptr(model));
            }

            inline void setAmbientUniform(const vec3 & ambient)
            {
                glUniform3fv(uniforms.ambient, 1, value_ptr(ambient));
            }

            inline void setEyePositionUniform(const vec3 & eyePosition)
            {
                glUniform3fv(uniforms.eyePosition, 1, value_ptr(eyePosition));
            }

            inline void setSpecularShininessUniform(const float& shininess)
            {
                glUniform1f(uniforms.material.specularShininess, shininess);
            }

            inline void setSpecularColorUniform(const glm::vec3 & specularColor)
            {
                glUniform3fv(uniforms.material.specularColor, 1, value_ptr(specularColor));
            }

            inline void enableAllAttributes()
            {
                glEnableVertexAttribArray(attributes.position);
                glEnableVertexAttribArray(attributes.color);
                glEnableVertexAttribArray(attributes.normal);
            }

            inline void disableAllAttributes()
            {
                glDisableVertexAttribArray(attributes.position);
                glDisableVertexAttribArray(attributes.color);
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
                    attributes.color,
                    Vertex::Color.Components,
                    Vertex::Color.Type,
                    Vertex::Color.Normalize,
                    sizeof(Vertex),
                    (void *)Vertex::Color.Offset
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
                printf("uniforms.projections.model %d\n", uniforms.projections.model);
                printf("uniforms.projections.view %d\n", uniforms.projections.view);
                printf("uniforms.projections.perspective %d\n", uniforms.projections.perspective);

                printf("uniforms.material.specularShininess %d\n", uniforms.material.specularShininess);
                printf("uniforms.material.specularColor %d\n", uniforms.material.specularColor);

                printf("uniforms.eyePosition %d\n", uniforms.eyePosition);
                printf("uniforms.ambient %d\n", uniforms.ambient);

                printf("attributes.position %d\n", attributes.position);
                printf("attributes.color %d\n", attributes.color);
                printf("attributes.normal %d\n", attributes.normal);
            }

            void acquireLayoutLocations(GLuint program)
            {
                uniforms.projections.model = glGetUniformLocation(program, "model_projection");
                uniforms.projections.view = glGetUniformLocation(program, "view_projection");
                uniforms.projections.perspective = glGetUniformLocation(program, "perspective_projection");

                uniforms.material.specularShininess = glGetUniformLocation(program, "material_specular_shininess");
                uniforms.material.specularColor = glGetUniformLocation(program, "material_specular_color");

                uniforms.eyePosition = glGetUniformLocation(program, "eye_position");
                uniforms.ambient = glGetUniformLocation(program, "ambient");

                attributes.position = glGetAttribLocation(program, "in_position");
                attributes.color = glGetAttribLocation(program, "in_color");
                attributes.normal = glGetAttribLocation(program, "in_normal");
            }
        };
    }
}

#endif // __TD_GPU_PROGRAMLAYOUT_H__
