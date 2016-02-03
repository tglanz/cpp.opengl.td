#ifndef __TD_GEOINFO_ICOSAHEDRON_H__
#define __TD_GEOINFO_ICOSAHEDRON_H__

#include <vector>

#include "../gpu/Vertex.h"

using namespace std;

using namespace td::gpu;

namespace td
{
    namespace geoInfo
    {
        namespace icosahedron
        {
            const unsigned int Indices[] =
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

			const float _x = 0.5257311121191336060f;
			const float _z = 0.8506508083520399320f;

			const Vertex Vertices[] =
			{
			        Vertex().setPosition(-_x, 0.0f, _z),
			        Vertex().setPosition(_x, 0.0f, _z),
			        Vertex().setPosition(-_x, 0.0f, -_z),
			        Vertex().setPosition(_x, 0.0f, -_z),
			        Vertex().setPosition(0.0f, _z, _x),
			        Vertex().setPosition(0.0f, _z, -_x),
			        Vertex().setPosition(0.0f, -_z, _x),
			        Vertex().setPosition(0.0f, -_z, -_x),
			        Vertex().setPosition(_z, _x, 0.0f),
			        Vertex().setPosition(-_z, _x, 0.0f),
			        Vertex().setPosition(_z, -_x, 0.0f),
			        Vertex().setPosition(-_z, -_x, 0.0f)
			};
        }
    }
}

#endif // __TD_GEOINFO_ICOSAHEDRON_H__
