#ifndef __TD_GPU_UBO_H__
#define __TD_GPU_UBO_H__

#include <stdio.h>
#include <cstring>
#include <GL/glew.h>

using namespace std;

namespace td
{
    namespace gpu
    {
        class UBO
        {
        private:
            bool m_initialized;

            size_t m_sizeofData;
            const void * m_dataPtr;
            const char * m_blockName;

            unsigned int m_blockIndex;
            unsigned int m_blockBindingIndex;

            GLuint m_ubo;

        public:

            UBO(const unsigned & bindingIndex, const char * blockName)
            {
                m_blockName = blockName;
                m_blockBindingIndex = bindingIndex;
            }

            bool initialize(GLuint program, size_t sizeofData, const void * dataPtr)
            {
                if (m_initialized) return false;

                m_sizeofData = sizeofData;
                m_dataPtr = dataPtr;

                m_initialized = true;
                glGenBuffers(1, &m_ubo);
                glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
                glBufferData(GL_UNIFORM_BUFFER, m_sizeofData, m_dataPtr, GL_DYNAMIC_DRAW);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                m_blockIndex = glGetUniformBlockIndex(program, m_blockName);

                glBindBufferBase(GL_UNIFORM_BUFFER, m_blockBindingIndex, m_ubo);
                glUniformBlockBinding(program, m_blockIndex, m_blockBindingIndex);

                return true;
            }

            void remap()
            {
                glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
                GLvoid * memoryAddress = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
                memcpy(memoryAddress, m_dataPtr, m_sizeofData);
                glUnmapBuffer(GL_UNIFORM_BUFFER);
            }
        };
    }
}

#endif // __TD_GPU_UBO_H__
