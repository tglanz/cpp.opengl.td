#include "Vertex.h"

using namespace td;
using namespace gpu;

const VertexAttribMeta Vertex::Position = VertexAttribMeta(sizeof(float), 3, 0, GL_FLOAT, GL_FALSE);
const VertexAttribMeta Vertex::Normal   = VertexAttribMeta(sizeof(float), 3, Vertex::Position.Size, GL_FLOAT, GL_FALSE);
