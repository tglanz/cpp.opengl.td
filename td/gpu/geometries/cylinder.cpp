#include "../geometries.h"

Geometry * geometries::newCylinder(
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

Geometry * geometries::newCylinderFlat(
    const float & bottomRadius,
    const float & topRadius,
    const float & height,
    const unsigned int & latitudeCuts)
{
    vector<Vertex> vertices;

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
    Vertex bottomCenterVertex = vertices[bottomCenterIdx];

    unsigned int topCenterIdx = bottomCenterIdx + 1;
    Vertex topCenterVertex = vertices[topCenterIdx];

    unsigned int totalNonFace = (vertices.size() - 2);

    vector<Vertex> finalVertices;
    for (unsigned int latCutIdx = 0; latCutIdx < latitudeCuts; ++latCutIdx)
    {
        unsigned int a, b, c, d;

        a = (latCutIdx * 4) % totalNonFace;
        b = (a + 2) % totalNonFace;
        c = (a + 4) % totalNonFace;
        d = (a + 6) % totalNonFace;

        Vertex va = vertices[a];
        Vertex vb = vertices[b];
        Vertex vc = vertices[c];
        Vertex vd = vertices[d];

        Vertex va1 = vertices[a + 1];
        Vertex vb1 = vertices[b + 1];
        Vertex vc1 = vertices[c + 1];
        Vertex vd1 = vertices[d + 1];

        finalVertices.push_back(vc);
        finalVertices.push_back(vb);
        finalVertices.push_back(va);

        finalVertices.push_back(vd);
        finalVertices.push_back(vb);
        finalVertices.push_back(vc);

        finalVertices.push_back(va1);
        finalVertices.push_back(bottomCenterVertex);
        finalVertices.push_back(vc1);

        finalVertices.push_back(topCenterVertex);
        finalVertices.push_back(vb1);
        finalVertices.push_back(vd1);
    }

    Geometry * retVal = new Geometry(finalVertices);
    retVal->computeNormals();

    return retVal;
}
