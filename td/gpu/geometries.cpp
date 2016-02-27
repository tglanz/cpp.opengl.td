/*
#include "geometries.h"

#include "../geoInfo/icosahedron.h"

#include "Vertex.h"
#include <vector>
#include <math.h>

using namespace td;
using namespace gpu;
using namespace geoInfo;

const float PI = 3.14159265359;

std::vector<Vertex> subdivide(Vertex a, Vertex b, Vertex c, const float & expectedNorm, const unsigned int & subdivisions)
{
    std::vector<Vertex> retVal;
    if (subdivisions == 0)
    {
        retVal.reserve(3);
        retVal.push_back(a);
        retVal.push_back(b);
        retVal.push_back(c);
    }
    else
    {
        glm::vec3 abMiddleVec = a.getPosition() + (b.getPosition() - a.getPosition()) / 2.0f;
        abMiddleVec *= expectedNorm / glm::length(abMiddleVec);

        glm::vec3 bcMiddleVec = b.getPosition() + (c.getPosition() - b.getPosition()) / 2.0f;
        bcMiddleVec *= expectedNorm / glm::length(bcMiddleVec);

        glm::vec3 caMiddleVec = c.getPosition() + (a.getPosition() - c.getPosition()) / 2.0f;
        caMiddleVec *= expectedNorm / glm::length(caMiddleVec);

        Vertex abMiddle = Vertex().setPosition(abMiddleVec);
        Vertex bcMiddle = Vertex().setPosition(bcMiddleVec);
        Vertex caMiddle = Vertex().setPosition(caMiddleVec);

        // TODO: Normal interpolation?

        Vertex vertices[] =
        {
            a, abMiddle, caMiddle,
            abMiddle, b, bcMiddle,
            bcMiddle, c, caMiddle,
            caMiddle, a, abMiddle,
            abMiddle, bcMiddle, caMiddle
        };

        // Note: Not very memory efficient, how to do better?
        // Perhaps pass a,b and c's indices as well, and return a modified indices

        retVal = std::vector<Vertex>(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
    }

    if (subdivisions > 1)
    {
        std::vector<Vertex> added = std::vector<Vertex>();

        for (unsigned int idx = 0; idx < retVal.size(); idx += 3)
        {
            std::vector<Vertex> subdivided = subdivide(retVal[idx], retVal[idx + 1], retVal[idx + 2], expectedNorm, subdivisions - 1);

            for (unsigned int idx2 = 0; idx2 < subdivided.size(); ++idx2)
            {
                added.push_back(subdivided[idx2]);
            }
        }

        retVal = added;
    }

    return retVal;
}

td::gpu::Geometry * td::gpu::geometries::newGeodesicSphere(const float & radius, const unsigned int & subdivisions)
{
    // OUCH

    std:vector<Vertex> vertices;

    unsigned int indicesCount = sizeof(icosahedron::Indices) / sizeof(icosahedron::Indices[0]);

    for (unsigned int indicesIdx = 0; indicesIdx < indicesCount; indicesIdx += 3)
    {
        unsigned int ia = icosahedron::Indices[indicesIdx];
        unsigned int ib = icosahedron::Indices[indicesIdx + 1];
        unsigned int ic = icosahedron::Indices[indicesIdx + 2];

        Vertex va = icosahedron::Vertices[ia];
        va.setPosition(radius * va.getPosition());

        Vertex vb = icosahedron::Vertices[ib];
        vb.setPosition(radius * vb.getPosition());

        Vertex vc = icosahedron::Vertices[ic];
        vc.setPosition(radius * vc.getPosition());

        std::vector<Vertex> subdivision = subdivide(va, vb, vc, radius, subdivisions);

        for (unsigned int subdivisionIdx = 0; subdivisionIdx < subdivision.size(); ++subdivisionIdx)
        {
            vertices.push_back(subdivision[subdivisionIdx]);
        }
    }

    Geometry * retVal = new Geometry(vertices);
    retVal->computeNormals();

    return retVal;
}

td::gpu::Geometry * td::gpu::geometries::newIcosahedron(const float & radius)
{
    std::vector<Vertex> vertices;
    unsigned int verticesCount = sizeof(icosahedron::Vertices) / sizeof(icosahedron::Vertices[0]);
    vertices.reserve(verticesCount);
    for (unsigned int idx = 0; idx < verticesCount; ++idx)
    {
        Vertex orgVertex = icosahedron::Vertices[idx];
        vertices.push_back(Vertex().setPosition(radius * orgVertex.getPosition()));
    }

    unsigned int sizeofIndices = sizeof(icosahedron::Indices);
    unsigned int sizeofIndex = sizeof(icosahedron::Indices[0]);
    unsigned int last = sizeofIndices / sizeofIndex;
    std::vector<unsigned int> indices;

    for (unsigned int idx = 0; idx < last; ++idx)
    {
        indices.push_back(icosahedron::Indices[idx]);
    }

    td::gpu::Geometry * retVal = new td::gpu::Geometry(vertices, indices);
    retVal->computeNormals();

    return retVal;
};

td::gpu::Geometry * td::gpu::geometries::newPlane(
                const float & width,
                const float & depth,
                const int & widthSegments,
                const int & depthSegments)
{
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;

    float segmentWidth = width / (float)widthSegments;
    float segmentDepth = depth / (float)depthSegments;

    std::vector<Vertex> vertices;

    for (int iz = 0; iz < depthSegments; ++iz)
    {
        float z = iz * segmentDepth - halfDepth;

        for (int ix = 0; ix < widthSegments; ++ix)
        {
            float x = ix * segmentWidth - halfWidth;

            Vertex vertex;
            vertex.setPosition(x, 0, z);

            vertices.push_back(vertex);
        }
    }

    std::vector<unsigned int> indices;

    for (int iz = 0; iz < depthSegments; ++iz)
    {
        for (int ix = 0; ix < widthSegments; ++ix)
        {
            unsigned int a =( ix + widthSegments * iz);
            unsigned int b =(ix + widthSegments * (iz + 1));
            unsigned int c =((ix + 1) + widthSegments * (iz + 1));
            unsigned int d =((ix + 1) + widthSegments * iz);

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    return new Geometry(vertices, indices);
}
*/
