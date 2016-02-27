#ifndef __TD_GPU_GEOMETRIES_H__
#define __TD_GPU_GEOMETRIES_H__

#include <vector>
#include <math.h>
#include <glm/glm.hpp>

#include "Geometry.h"
#include "Vertex.h"

using namespace td;
using namespace gpu;
using namespace glm;

namespace td
{
    namespace gpu
    {
        namespace geometries
        {
            const float PI = 3.14159265359;

            Geometry * newPlane(
                const float & width,
                const float & height,
                const int & widthSegments,
                const int & heightSegments);

            Geometry * newPlaneFlat(
                const float & width,
                const float & height,
                const int & widthSegments,
                const int & heightSegments);

            Geometry * newIcosahedron(const float & radius);
            Geometry * newIcosahedronFlat(const float & radius);

            Geometry * newGeodesicSphere(const float & radius, const unsigned int & subdivisions);
            Geometry * newGeodesicSphereFlat(const float & radius, const unsigned int & subdivisions);

            Geometry * newCylinder(const float & bottomRadius, const float & topRadius, const float & height, const unsigned int & latitudeCuts);
            Geometry * newCylinderFlat(const float & bottomRadius, const float & topRadius, const float & height, const unsigned int & latitudeCuts);

            Geometry * newBox(const float & width, const float & height, const float & depth);

        }
    }
}

#endif // __TD_GPU_GEOMETRIES_H__
