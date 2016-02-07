#include "geometries.h"

#include "../geoInfo/icosahedron.h"

#include "Vertex.h"
#include <vector>
#include <math.h>

using namespace td;
using namespace gpu;
using namespace geoInfo;

const float PI = 3.14159265359;

td::gpu::Geometry * td::gpu::geometries::newTemporary(const float& a, const float& b)
{
    Vertex vertices[] =
    {
        Vertex().setPosition(-4.0f + a, -4.0f + b, 0.0),
        Vertex().setPosition(4.0f + a, -4.0f + b, 0.0),
        Vertex().setPosition(0.0f + a, 4.0f + b, 0.0)
    };

    Geometry * geometry = new Geometry(vertices, 3);

    return geometry;
}

td::gpu::Geometry * td::gpu::geometries::newCylinder(
    const float & bottomRadius,
    const float & topRadius,
    const float & height,
    const unsigned int & latitudeCuts)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float halfHeight = height / 2.0f;
    float bottomY = -halfHeight;
    float topY = halfHeight;

    float radsPart = 2.0f * PI / latitudeCuts;

    for (unsigned int latCutIdx = 0; latCutIdx < latitudeCuts; ++latCutIdx)
    {
        float latCutRads = radsPart * latCutIdx;

        float cosine = cos(latCutRads);
        float sinuse = sin(latCutRads);

        float bottomX = bottomRadius * cosine;
        float bottomZ = bottomRadius * sinuse;

        float topX = topRadius * cosine;
        float topZ = topRadius * sinuse;

        Vertex bottomVertex = Vertex().setPosition(bottomX, bottomY, bottomZ).setNormal(bottomX, 0, bottomZ);
        Vertex bottomVertexFace = Vertex().setPosition(bottomX, bottomY, bottomZ).setNormal(0, -1.0f, 0);
        Vertex topVertex = Vertex().setPosition(topX, topY, topZ).setNormal(topX, 0, topZ);
        Vertex topVertexFace = Vertex().setPosition(topX, topY, topZ).setNormal(0, 1.0f, 0);

        vertices.push_back(bottomVertex);
        vertices.push_back(bottomVertexFace);
        vertices.push_back(topVertex);
        vertices.push_back(topVertexFace);
    }

    vertices.push_back(Vertex().setPosition(0, -halfHeight, 0).setNormal(0, -1.0f, 0)); // center bottom
    vertices.push_back(Vertex().setPosition(0, halfHeight, 0).setNormal(0, 1.0f, 0)); // center top

    unsigned int bottomCenterIdx = vertices.size() - 2;
    unsigned int topCenterIdx = bottomCenterIdx + 1;

    unsigned int totalNonFace = (vertices.size() - 2);

    for (unsigned int latCutIdx = 0; latCutIdx < latitudeCuts; ++latCutIdx)
    {
        unsigned int a, b, c, d;

        a = (latCutIdx * 4) % totalNonFace;
        b = (a + 2) % totalNonFace;
        c = (a + 4) % totalNonFace;
        d = (a + 6) % totalNonFace;

        indices.push_back(c);
        indices.push_back(b);
        indices.push_back(a);

        indices.push_back(d);
        indices.push_back(b);
        indices.push_back(c);

        indices.push_back(a + 1);
        indices.push_back(bottomCenterIdx);
        indices.push_back(c + 1);

        indices.push_back(topCenterIdx);
        indices.push_back(b + 1);
        indices.push_back(d + 1);
    }

    Geometry * retVal = new Geometry(vertices, indices);

    return retVal;
}

td::gpu::Geometry * td::gpu::geometries::newIcosahedron()
{
    std::vector<Vertex> vertices(
         icosahedron::Vertices,
         icosahedron::Vertices + sizeof(icosahedron::Vertices) / sizeof(icosahedron::Vertices[0])
    );

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
