#ifndef __TD_GPU_SHADERSTRUCTS_H__
#define __TD_GPU_SHADERSTRUCTS_H__

namespace td
{
    namespace gpu
    {
        namespace shaderStructs
        {
            struct AttenuationFactors
            {
                float constant = 1;
                float linear = .16;
                float quadratic = 0.08;
            };

            struct PointLight
            {
                vec4 position = vec4(0, 0, 0, 0);
                vec4 intensities = vec4(0, 0, 0, 0);
                AttenuationFactors attenuation;
                float reserved_1;
            };

            struct DirectionalLight
            {
                vec4 direction = vec4(0, 0, 0, 0);
                vec4 intensities = vec4(0, 0, 0, 0);
            };
        }
    }
}

#endif // __TD_GPU_SHADERSTRUCTS_H__
