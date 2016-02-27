#include "../geometries.h"

Geometry * geometries::newPlane(
                const float & width,
                const float & depth,
                const int & widthSegments,
                const int & depthSegments)
{
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;

    float segmentWidth = width / widthSegments;
    float segmentDepth = depth / depthSegments;

    std::vector<Vertex> vertices;

    for (int iz = 0; iz < depthSegments; ++iz)
    {
        float z = iz * segmentDepth - halfDepth;

        for (int ix = 0; ix < widthSegments; ++ix)
        {
            float x = ix * segmentWidth - halfWidth;
            vertices.push_back(Vertex().setPosition(x, 0, z).setNormal(0, 1, 0));
        }
    }

    vector<unsigned int> indices;

    for (int iz = 0; iz < depthSegments - 1; ++iz)
    {
        for (int ix = 0; ix < widthSegments - 1; ++ix)
        {
            /*
                c   d

                a   b
            */

            unsigned int a = (ix + widthSegments * iz);
            unsigned int b = (ix + 1 + widthSegments * iz);
            unsigned int c = (ix + widthSegments * (iz + 1));
            unsigned int d = (ix + 1 + widthSegments * (iz + 1));

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    Geometry * retVal = new Geometry(vertices, indices);
    return retVal;
}

Geometry * geometries::newPlaneFlat(
                const float & width,
                const float & depth,
                const int & widthSegments,
                const int & depthSegments)
{
    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;

    float segmentWidth = width / widthSegments;
    float segmentDepth = depth / depthSegments;

    std::vector<Vertex> vertices;

    for (int iz = 0; iz < depthSegments; ++iz)
    {
        float z = iz * segmentDepth - halfDepth;

        for (int ix = 0; ix < widthSegments; ++ix)
        {
            float x = ix * segmentWidth - halfWidth;
            vertices.push_back(Vertex().setPosition(x, 0, z).setNormal(0, 1, 0));
        }
    }

    vector<Vertex> finalVertices;
    vector<unsigned int> indices;

    for (int iz = 0; iz < depthSegments - 1; ++iz)
    {
        for (int ix = 0; ix < widthSegments - 1; ++ix)
        {
            unsigned int a = (ix + widthSegments * iz);
            unsigned int b = (ix + 1 + widthSegments * iz);
            unsigned int c = (ix + widthSegments * (iz + 1));
            unsigned int d = (ix + 1 + widthSegments * (iz + 1));

            finalVertices.push_back(vertices[a].setNormal(0, 1, 0));
            finalVertices.push_back(vertices[b].setNormal(0, 1, 0));
            finalVertices.push_back(vertices[c].setNormal(0, 1, 0));

            finalVertices.push_back(vertices[b].setNormal(0, 1, 0));
            finalVertices.push_back(vertices[c].setNormal(0, 1, 0));
            finalVertices.push_back(vertices[d].setNormal(0, 1, 0));
        }
    }

    Geometry * retVal = new Geometry(finalVertices);
    return retVal;
}
