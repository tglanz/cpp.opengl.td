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

            td::gpu::Geometry * newIcosahedron();

            td::gpu::Geometry * newTemporary(const float& a, const float& b);
        }
    }
}

#endif // __TD_GPU_GEOMETRIES_H__
