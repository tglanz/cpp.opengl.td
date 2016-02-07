#ifndef __TD_GPU_PROGRAM_H__
#define __TD_GPU_PROGRAM_H__

#include <GL/glew.h>
#include <string>

#include <iostream>
#include <fstream>
#include <streambuf>

#include "ProgramLayout.h"

#include "../log.h"

namespace td
{
    namespace gpu
    {
        class Program
        {
        private:
            GLuint m_programId;

            ProgramLayout m_layout;

            struct
            {
                std::string vertex = "";
                std::string fragment = "";
            } m_shaderPaths;

            bool compileShader(GLuint shader);
            GLuint createShaderFromSource(GLuint shaderType, std::string source);
            GLuint createShaderFromFile(GLuint shaderType, std::string filePath);
            void deleteShader(GLuint shader);

        public:

            Program(
                const std::string & vertexPath,
                const std::string & fragmentPath
            )
            {
                m_shaderPaths.vertex = vertexPath;
                m_shaderPaths.fragment = fragmentPath;
            }

            ~Program()
            {
                if (m_programId)
                {
                    char num[15];
                    sprintf(num, "%d", m_programId);

                    glDeleteProgram(m_programId);

                    td::log::info("Program deleted: " + string(num));
                }
            }

            void use()
            {
                glUseProgram(m_programId);
            }

            bool compile();

            ProgramLayout * const layout() { return &m_layout; }

            void acquireLayoutLocations()
            {
                m_layout.acquireLayoutLocations(m_programId);
                m_layout.print();
            }
        };
    }
}

#endif // __TD_GPU_PROGRAM_H__
