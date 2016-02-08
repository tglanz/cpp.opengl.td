#ifndef __TD_GPU_SHADERBLOCKS_H__
#define __TD_GPU_SHADERBLOCKS_H__

#include "shaderStructs.h"

using namespace td::gpu::shaderStructs;

namespace td
{
    namespace gpu
    {
        namespace shaderBlocks
        {
            const int MAX_POINT_LIGHTS = 5;
            const int MAX_DIRECTIONAL_LIGHTS = 4;

            struct ViewBlock
            {
                mat4 viewProjection;
                mat4 perspectiveProjection;
                vec3 eyePosition;
            };

            struct MaterialSpecularBlock
            {
                vec3 color;
                float shininess;
            };

            struct MaterialDiffuesBlock
            {
                vec3 color;
                float shininess;
            };

            struct PointLightBlock
            {
                PointLight lights[MAX_POINT_LIGHTS];
                unsigned int sequentialNumberOfLights = 0;
            };

            struct DirectionalLightBlock
            {
                DirectionalLight lights[MAX_DIRECTIONAL_LIGHTS];
                unsigned int sequentialNumberOfLights = 0;
            };
        }
    }
}

#endif // __TD_GPU_SHADERBLOCKS_H__
