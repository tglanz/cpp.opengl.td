#include "../geometries.h"

// Possibly a duplicate of icosahedron data,
// though we dont want to create any dependency,
// this is only the current algorithm for sphere generation

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

// FLAT (DISCO BALL)

vector<Vertex> subdivide(Vertex a, Vertex b, Vertex c, const float & expectedNorm, const unsigned int & subdivisions)
{
    vector<Vertex> retVal;
    if (subdivisions == 0)
    {
        retVal.reserve(3);
        retVal.push_back(a);
        retVal.push_back(b);
        retVal.push_back(c);
    }
    else
    {
        vec3 abMiddleVec = a.getPosition() + (b.getPosition() - a.getPosition()) / 2.0f;
        vec3 bcMiddleVec = b.getPosition() + (c.getPosition() - b.getPosition()) / 2.0f;
        vec3 caMiddleVec = c.getPosition() + (a.getPosition() - c.getPosition()) / 2.0f;

        Vertex abMiddle = Vertex().setPosition(abMiddleVec).setNorm(expectedNorm);
        Vertex bcMiddle = Vertex().setPosition(bcMiddleVec).setNorm(expectedNorm);
        Vertex caMiddle = Vertex().setPosition(caMiddleVec).setNorm(expectedNorm);

        Vertex vertices[] =
        {
            a, abMiddle, caMiddle,
            abMiddle, b, bcMiddle,
            bcMiddle, c, caMiddle,
            caMiddle, a, abMiddle,
            abMiddle, bcMiddle, caMiddle
        };

        retVal = vector<Vertex>(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
    }

    if (subdivisions > 1)
    {
        vector<Vertex> added = vector<Vertex>();

        for (unsigned int idx = 0; idx < retVal.size(); idx += 3)
        {
            vector<Vertex> subdivided = subdivide(retVal[idx], retVal[idx + 1], retVal[idx + 2], expectedNorm, subdivisions - 1);

            for (Vertex vertex : subdivided)
                added.push_back(vertex);
        }

        retVal = added;
    }

    return retVal;
}


Geometry * geometries::newGeodesicSphereFlat(const float & radius, const unsigned int & subdivisions)
{
    unsigned int toSubdivide = subdivisions;

    // maximum subdivisions until better we create a better algorithm
    // Though, if we wont need a flat sphere, use smooth sphere
    if (toSubdivide >= 6)
    {
        toSubdivide = 6;
    }

    vector<Vertex> vertices;

    unsigned int indicesCount = sizeof(INDICES) / sizeof(INDICES[0]);

    for (unsigned int indicesIdx = 0; indicesIdx < indicesCount; indicesIdx += 3)
    {
        unsigned int ia = INDICES[indicesIdx];
        unsigned int ib = INDICES[indicesIdx + 1];
        unsigned int ic = INDICES[indicesIdx + 2];

        Vertex va = VERTICES[ia];
        va.setPosition(radius * va.getPosition());

        Vertex vb = VERTICES[ib];
        vb.setPosition(radius * vb.getPosition());

        Vertex vc = VERTICES[ic];
        vc.setPosition(radius * vc.getPosition());

        vector<Vertex> subdivision = subdivide(va, vb, vc, radius, toSubdivide);

        for (unsigned int subdivisionIdx = 0; subdivisionIdx < subdivision.size(); ++subdivisionIdx)
        {
            vertices.push_back(subdivision[subdivisionIdx]);
        }
    }

    Geometry * retVal = new Geometry(vertices);
    retVal->computeNormals();

    return retVal;
}

// SMOOTH (INDEXED)

struct IndexTriplet
{
    unsigned int i1, i2, i3;
};

void subdivide(const unsigned int & tripletIndex, vector<Vertex> & vertices, vector<IndexTriplet> & triplets, const float & expectedNorm)
{
    IndexTriplet triplet = triplets[tripletIndex];

    unsigned int ai = triplet.i1;
    unsigned int bi = triplet.i2;
    unsigned int ci = triplet.i3;

    Vertex a = vertices[ai];
    Vertex b = vertices[bi];
    Vertex c = vertices[ci];

    vec3 abMiddleVec = a.getPosition() + (b.getPosition() - a.getPosition()) / 2.0f;
    vec3 bcMiddleVec = b.getPosition() + (c.getPosition() - b.getPosition()) / 2.0f;
    vec3 caMiddleVec = c.getPosition() + (a.getPosition() - c.getPosition()) / 2.0f;

    Vertex abMiddle = Vertex().setPosition(abMiddleVec).setNorm(expectedNorm);
    Vertex bcMiddle = Vertex().setPosition(bcMiddleVec).setNorm(expectedNorm);
    Vertex caMiddle = Vertex().setPosition(caMiddleVec).setNorm(expectedNorm);

    // xyi = xyMiddle index
    unsigned int abi = vertices.size();
    unsigned int bci = abi + 1;
    unsigned int cai = bci + 1;

    vertices.push_back(abMiddle);
    vertices.push_back(bcMiddle);
    vertices.push_back(caMiddle);

    IndexTriplet a2abm2cam;
    a2abm2cam.i1 = ai;
    a2abm2cam.i2 = abi;
    a2abm2cam.i3 = cai;

    IndexTriplet abm2b2bcm;
    abm2b2bcm.i1 = abi;
    abm2b2bcm.i2 = bi;
    abm2b2bcm.i3 = bci;

    IndexTriplet bcm2c2cam;
    bcm2c2cam.i1 = bci;
    bcm2c2cam.i2 = ci;
    bcm2c2cam.i3 = cai;

    IndexTriplet abm2bcm2cam;
    abm2bcm2cam.i1 = abi;
    abm2bcm2cam.i2 = bci;
    abm2bcm2cam.i3 = cai;

    triplets[tripletIndex] = a2abm2cam;
    triplets.push_back(abm2b2bcm);
    triplets.push_back(bcm2c2cam);
    triplets.push_back(abm2bcm2cam);
}

Geometry * geometries::newGeodesicSphere(const float & radius, const unsigned int & subdivisions)
{
    vector<Vertex> vertices;
    vector<IndexTriplet> triplets;

    const unsigned int indicesCount = sizeof(INDICES) / sizeof(INDICES[0]);
    for (unsigned int idx = 0; idx < indicesCount; idx += 3)
    {
        IndexTriplet triplet;
        triplet.i1 = INDICES[idx];
        triplet.i2 = INDICES[idx + 1];
        triplet.i3 = INDICES[idx + 2];
        triplets.push_back(triplet);
    }

    const unsigned int verticesCount = sizeof(VERTICES) / sizeof(VERTICES[0]);
    for (unsigned int idx = 0; idx < verticesCount; ++idx)
        vertices.push_back(VERTICES[idx]);

    for (unsigned int subdivisionRound = 0; subdivisionRound < subdivisions; ++subdivisionRound)
    {
        unsigned int tripletsCount = triplets.size();
        for (unsigned int tripletIndex = 0; tripletIndex < tripletsCount; ++tripletIndex)
        {
            subdivide(tripletIndex, vertices, triplets, radius);
        }
    }

    vector<unsigned int> indices;
    for (IndexTriplet triplet : triplets)
    {
        indices.push_back(triplet.i1);
        indices.push_back(triplet.i2);
        indices.push_back(triplet.i3);
    }

    Geometry * retVal = new Geometry(vertices, indices);
    retVal->computeNormals();
    return retVal;
}
