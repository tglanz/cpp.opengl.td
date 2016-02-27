#include "../geometries.h"

td::gpu::Geometry * td::gpu::geometries::newBox(const float & width, const float & height, const float & depth)
{
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float halfD = depth / 2.0f;

    Vertex vertices[] =
    {
        // FRONT FACE BOTTOM TRIANGLE
        Vertex().setPosition(-halfW, -halfH, -halfD).setNormal(0, 0, -1),
        Vertex().setPosition(halfW, -halfH, -halfD).setNormal(0, 0, -1),
        Vertex().setPosition(-halfW, halfH, -halfD).setNormal(0, 0, -1),

        // FRONT FACE UPPER TRAINGLE
        Vertex().setPosition(halfW, halfH, -halfD).setNormal(0, 0, -1),
        Vertex().setPosition(halfW, -halfH, -halfD).setNormal(0, 0, -1),
        Vertex().setPosition(-halfW, halfH, -halfD).setNormal(0, 0, -1),

        // BACK FACE BOTTOM TRIANGLE
        Vertex().setPosition(-halfW, -halfH, halfD).setNormal(0, 0, 1),
        Vertex().setPosition(halfW, -halfH, halfD).setNormal(0, 0, 1),
        Vertex().setPosition(-halfW, halfH, halfD).setNormal(0, 0, 1),

        // BACK FACE UPPER TRAINGLE
        Vertex().setPosition(halfW, halfH, halfD).setNormal(0, 0, 1),
        Vertex().setPosition(halfW, -halfH, halfD).setNormal(0, 0, 1),
        Vertex().setPosition(-halfW, halfH, halfD).setNormal(0, 0, 1),

        // RIGHT FACE BOTTOM TRIANGLE
        Vertex().setPosition(halfW, -halfH, -halfD).setNormal(1, 0, 0),
        Vertex().setPosition(halfW, -halfH, halfD).setNormal(1, 0, 0),
        Vertex().setPosition(halfW, halfH, -halfD).setNormal(1, 0, 0),

        // RIGHT FACE TOP TRIANGLE
        Vertex().setPosition(halfW, -halfH, halfD).setNormal(1, 0, 0),
        Vertex().setPosition(halfW, halfH, halfD).setNormal(1, 0, 0),
        Vertex().setPosition(halfW, halfH, -halfD).setNormal(1, 0, 0),

        // LEFT FACE BOTTOM TRIANGLE
        Vertex().setPosition(-halfW, -halfH, -halfD).setNormal(-1, 0, 0),
        Vertex().setPosition(-halfW, -halfH, halfD).setNormal(-1, 0, 0),
        Vertex().setPosition(-halfW, halfH, -halfD).setNormal(-1, 0, 0),

        // LEFT FACE TOP TRIANGLE
        Vertex().setPosition(-halfW, -halfH, halfD).setNormal(-1, 0, 0),
        Vertex().setPosition(-halfW, halfH, halfD).setNormal(-1, 0, 0),
        Vertex().setPosition(-halfW, halfH, -halfD).setNormal(-1, 0, 0),

        // BOTTOM FACE BOTTOM TRIANGLE
        Vertex().setPosition(-halfW, -halfH, -halfD).setNormal(0, -1, 0),
        Vertex().setPosition(halfW, -halfH, -halfD).setNormal(0, -1, 0),
        Vertex().setPosition(-halfW, -halfH, halfD).setNormal(0, -1, 0),

        // BOTTOM FACE TOP TRIANGLE
        Vertex().setPosition(halfW, -halfH, -halfD).setNormal(0, -1, 0),
        Vertex().setPosition(halfW, -halfH, halfD).setNormal(0, -1, 0),
        Vertex().setPosition(-halfW, -halfH, halfD).setNormal(0, -1, 0),

        // TOP FACE BOTTOM TRIANGLE
        Vertex().setPosition(-halfW, halfH, -halfD).setNormal(0, 1, 0),
        Vertex().setPosition(halfW, halfH, -halfD).setNormal(0, 1, 0),
        Vertex().setPosition(-halfW, halfH, halfD).setNormal(0, 1, 0),

        // TOP FACE TOP TRIANGLE
        Vertex().setPosition(halfW, halfH, -halfD).setNormal(0, 1, 0),
        Vertex().setPosition(halfW, halfH, halfD).setNormal(0, 1, 0),
        Vertex().setPosition(-halfW, halfH, halfD).setNormal(0, 1, 0),

    };

    return new td::gpu::Geometry(vertices, sizeof(vertices) / sizeof(Vertex));
}
