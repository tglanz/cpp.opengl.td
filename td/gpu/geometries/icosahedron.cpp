#include "../geometries.h"

const unsigned int INDICES[] =
{
    0, 4, 1,
    0, 9, 4,
    9, 5, 4,
    4, 5, 8,
    4, 8, 1,
    8, 10, 1,
    8, 3, 10,
    5, 3, 8,
    5, 2, 3,
    2, 7, 3,
    7, 10, 3,
    7, 6, 10,
    7, 11, 6,
    11, 0, 6,
    0, 1, 6,
    6, 1, 10,
    9, 0, 11,
    9, 11, 2,
    9, 2, 5,
    7, 2, 11
};

const float X = 0.5257311121191336060f;
const float Z = 0.8506508083520399320f;

const Vertex VERTICES[] =
{
        Vertex().setPosition(-X, 0.0f, Z),
        Vertex().setPosition(X, 0.0f, Z),
        Vertex().setPosition(-X, 0.0f, -Z),
        Vertex().setPosition(X, 0.0f, -Z),
        Vertex().setPosition(0.0f, Z, X),
        Vertex().setPosition(0.0f, Z, -X),
        Vertex().setPosition(0.0f, -Z, X),
        Vertex().setPosition(0.0f, -Z, -X),
        Vertex().setPosition(Z, X, 0.0f),
        Vertex().setPosition(-Z, X, 0.0f),
        Vertex().setPosition(Z, -X, 0.0f),
        Vertex().setPosition(-Z, -X, 0.0f)
};

td::gpu::Geometry* td::gpu::geometries::newIcosahedron(const float & radius)
{
    vector<Vertex> vertices;
    for (Vertex vertex : VERTICES)
        vertices.push_back(vertex.setNorm(radius));

    vector<unsigned int> indices;

    for (unsigned int index : INDICES)
        indices.push_back(index);

    Geometry * retVal = new Geometry(vertices, indices);
    retVal->computeNormals();

    return retVal;
}

td::gpu::Geometry* td::gpu::geometries::newIcosahedronFlat(const float & radius)
{
    vector<Vertex> vertices;

    int numIndices = sizeof(INDICES) / sizeof(INDICES[0]);
    for (int idx = 0; idx < numIndices; idx += 3)
    {
        // vi: vertex index
        unsigned int vi1 = INDICES[idx];
        unsigned int vi2 = INDICES[idx + 1];
        unsigned int vi3 = INDICES[idx + 2];

        // v: vertex
        Vertex v1 = VERTICES[vi1];
        Vertex v2 = VERTICES[vi2];
        Vertex v3 = VERTICES[vi3];

        vec3 normal = (v1.getPosition() + v2.getPosition() + v3.getPosition()) / 3.0f;

        vertices.push_back(v1.setNorm(radius).setNormal(normal));
        vertices.push_back(v2.setNorm(radius).setNormal(normal));
        vertices.push_back(v3.setNorm(radius).setNormal(normal));
    }

    Geometry * retVal = new Geometry(vertices);

    return retVal;
}
