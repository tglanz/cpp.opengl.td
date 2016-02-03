#include "Vertex.h"

using namespace td;
using namespace gpu;

// Reminder: VertexAttribMeta(const int& componentSize, const int& components, const int& offset, GLuint type, GLboolean normalize):

const VertexAttribMeta Vertex::Position = VertexAttribMeta(sizeof(float), 3, 0, GL_FLOAT, GL_FALSE);
const VertexAttribMeta Vertex::Color    = VertexAttribMeta(sizeof(float), 4, Vertex::Position.Size, GL_FLOAT, GL_FALSE);
const VertexAttribMeta Vertex::Normal   = VertexAttribMeta(sizeof(float), 3, Vertex::Position.Size + Vertex::Color.Size, GL_FLOAT, GL_FALSE);
