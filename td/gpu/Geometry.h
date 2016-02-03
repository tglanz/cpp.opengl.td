#ifndef __TD_GPU_GEOMETRY_H__
#define __TD_GPU_GEOMETRY_H__

#include <vector>
#include <glm/glm.hpp>

#include "Vertex.h"

using namespace std;

namespace td
{
    namespace gpu
    {
        class Geometry
        {
        private:
            vector<Vertex> m_vertices;

            vector<unsigned int> m_indices;

            // Vertex Array Object
            GLuint m_vao;

            // Vertex Buffer Object
            GLuint m_vbo;

            // Vertex Element (array) Object
            GLuint m_veo;

            void updateVertices()
            {
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
                glBufferData(GL_ARRAY_BUFFER, getRawVerticesSize(), (void*)&m_vertices[0], GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            void updateIndices()
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, getRawIndicesSize(), (void*)&m_indices[0], GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

        protected:
            void initialize(vector<Vertex> vertices, vector<unsigned int> indices)
            {
                m_vao = 0;

                m_vertices = vertices;

                m_indices = indices;
                if (m_indices.empty())
                {
                    for (unsigned int idx = 0; idx < m_vertices.size(); ++idx)
                    {
                        m_indices.push_back(idx);
                    }
                }
            }

        public:
            ~Geometry()
            {
                glDeleteVertexArrays(1, &m_vao);
                glDeleteBuffers(1, &m_veo);
                glDeleteBuffers(1, &m_vbo);
            }

            Geometry(vector<Vertex> vertices, vector<unsigned int> indices)
            {
                initialize(vertices, indices);
            }

            Geometry(vector<Vertex> vertices) :
                Geometry(vertices, vector<unsigned int>())
            { }

            Geometry(const Vertex* vertices, unsigned int count) :
                Geometry(vector<Vertex>(vertices, vertices + count))
            { }

            Geometry(
                 const Vertex* vertices, unsigned int verticesCount,
                 const unsigned int* indices, unsigned int indicesCount) :
                 Geometry(vector<Vertex>(vertices, vertices + verticesCount),
                          vector<unsigned int>(indices, indices + indicesCount))
            { }

            void printPositions()
            {
                printf("Geo Positions:\n");
                for (vector<Vertex>::iterator iter = m_vertices.begin(); iter != m_vertices.end(); iter++)
                {
                    Vertex vertex = *iter;

                    vertex.printPosition();
                }
            }

            void printIndices()
            {
                printf("Geo Indices:\n");
                for (unsigned int idx = 0; idx < m_indices.size(); ++idx)
                {
                    if (idx != 0 && idx % 3 == 0)
                    {
                        printf("\n");
                    }

                    printf("%d\t", m_indices[idx]);
                }
                printf("\n");
            }

            void printNormals()
            {
                printf("Geo Normals:\n");
                for (vector<Vertex>::iterator iter = m_vertices.begin(); iter != m_vertices.end(); iter++)
                {
                    Vertex vertex = *iter;

                    vertex.printNormal();
                }
            }

            void bind()
            {
                glBindVertexArray(m_vao);
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veo);
            }

            void unbind()
            {
                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            void setupGpuBindings()
            {
                if (m_vao == 0)
                {
                    glGenVertexArrays(1, &m_vao);

                    glBindVertexArray(m_vao);

                    glGenBuffers(1, &m_vbo);

                    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

                    glGenBuffers(1, &m_veo);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_veo);

                    updateVertices();

                    updateIndices();
                }
            }

            const vector<Vertex>& getVertices() const { return m_vertices; }
            const vector<unsigned int>& getIndices() const { return m_indices; }

            void computeNormals()
            {
                glm::vec3 vec0 = glm::vec3(0, 0, 0);
                for (unsigned int idx = 0; idx < m_vertices.size(); ++idx)
                {
                    m_vertices[idx].setNormalUnormalized(vec0);
                }

                for (unsigned int idx = 0; idx < m_indices.size(); idx += 3)
                {
                    unsigned int ia = m_indices[idx];
                    unsigned int ib = m_indices[idx + 1];
                    unsigned int ic = m_indices[idx + 2];

                    vec3 ba = m_vertices[ia].getPosition() - m_vertices[ib].getPosition();
                    vec3 bc = m_vertices[ic].getPosition() - m_vertices[ib].getPosition();
                    vec3 normal = cross(ba, bc);

                    m_vertices[ia].setNormalUnormalized(m_vertices[ia].getNormal() + normal);
                    m_vertices[ib].setNormalUnormalized(m_vertices[ib].getNormal() + normal);
                    m_vertices[ic].setNormalUnormalized(m_vertices[ic].getNormal() + normal);
                }

                // Normalize
                for (unsigned int idx = 0; idx < m_vertices.size(); ++idx)
                {
                    m_vertices[idx].setNormal(m_vertices[idx].getNormal());
                }
            }

            const unsigned int getRawVerticesSize() const { return m_vertices.size() * sizeof(Vertex); }
            const unsigned int getRawIndicesSize() const
            {
                unsigned int retVal = m_indices.size() * sizeof(unsigned int);

                return retVal;
            }

            const void* verticesPtr() const { return (void*)&m_vertices[0]; }
            const void* indicesPtr() const { return (void*)&m_indices[0]; }

            const GLuint& vao() const { return m_vao; }
            const GLuint& vbo() const { return m_vbo; }
            const GLuint& veo() const { return m_veo; }

            void drawElements()
            {
                glDrawElements(
                   GL_TRIANGLES,
                   m_indices.size(),
                   GL_UNSIGNED_INT,
                   (void*)0
                );
            }

            void drawElementsAsWireframe()
            {
                glDrawElements(
                   GL_LINES,
                   m_indices.size(),
                   GL_UNSIGNED_INT,
                   (void*)0
                );
            }
        };
    };
}

#endif // __TD_GPU_GEOMETRY_H__
