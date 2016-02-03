#include "geometries.h"

#include "../geoInfo/icosahedron.h"

#include "Vertex.h"
#include <vector>

using namespace td;
using namespace gpu;
using namespace geoInfo;

td::gpu::Geometry * td::gpu::geometries::newTemporary(const float& a, const float& b)
{
    Vertex vertices[] =
    {
        Vertex().setPosition(-4.0f + a, -4.0f + b, 0.0).setColor(0.0, 1.0, 0.0),
        Vertex().setPosition(4.0f + a, -4.0f + b, 0.0).setColor(1.0, 0.0, 0.0),
        Vertex().setPosition(0.0f + a, 4.0f + b, 0.0).setColor(0.0, 0.0, 1.0)
    };

    Geometry * geometry = new Geometry(vertices, 3);

    return geometry;
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
