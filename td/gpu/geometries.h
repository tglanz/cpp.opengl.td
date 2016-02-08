#ifndef __TD_GPU_GEOMETRIES_H__
#define __TD_GPU_GEOMETRIES_H__

#include "Geometry.h"

namespace td
{
    namespace gpu
    {
        namespace geometries
        {
            td::gpu::Geometry * newPlane(
                const float & width,
                const float & height,
                const int & widthSegments,
                const int & heightSegments);

            td::gpu::Geometry * newIcosahedron(const float & radius);
            td::gpu::Geometry * newGeodesicSphere(const float & radius, const unsigned int & subdivisions);
            td::gpu::Geometry * newTemporary(const float & a, const float& b);
            td::gpu::Geometry * newCylinder(const float & bottomRadius, const float & topRadius, const float & height, const unsigned int & latitudeCuts);
            td::gpu::Geometry * newBox(const float & width, const float & height, const float & depth);
        }
    }
}

#endif // __TD_GPU_GEOMETRIES_H__
