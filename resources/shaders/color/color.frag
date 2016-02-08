#version 430 core

layout(std140) uniform ViewBlock
{
    mat4 viewProjection;
    mat4 perspectiveProjection;
    vec3 eyePosition;
} viewBlock;

layout(std140) uniform MaterialDiffuseBlock
{
    vec3 color;
} materialDiffuseBlock;

layout(std140) uniform MaterialSpecularBlock
{
    vec3 color;
    float shininess;
} materialSpecularBlock;

struct PointLight
{
    vec4 position;
    vec4 intensities;
    float constantAttenuationFactor;
    float linearAttenuationFactor;
    float quadraticAttenuationFactor;
    float reserved_1;
};

struct DirectionalLight
{
    vec4 direction;
    vec4 intensities;
};

layout(std140) uniform PointLightBlock
{
    PointLight pointLights[5];
    int numPointLights;
};

layout(std140) uniform DirectionalLightBlock
{
    DirectionalLight directionalLights[4];
    int numDirectionalLights;
};

in VertexOutput
{
    vec3 position;
    vec3 normal;
} vertexOutput;

uniform vec3 ambient;

out vec4 color;

vec3 PointLightColor(vec3 surfaceNormal, PointLight light)
{
    vec3 diffuseIntensities = vec3(0, 0, 0);
    vec3 specularIntensities = vec3(0, 0, 0);

    vec3 pointLightIntensities = light.intensities.rgb;
    vec3 surfaceToLight = light.position.xyz - vertexOutput.position;
    vec3 surfaceToLightDirection = normalize(surfaceToLight);
    float distanceSurfaceToLight = length(surfaceToLight);

    // diffuse
    float cosAoiAngle = dot(surfaceNormal, surfaceToLightDirection);
    float diffuseCoefficient = max(0.0f, cosAoiAngle);

    diffuseIntensities = diffuseCoefficient * pointLightIntensities * materialDiffuseBlock.color;

    // specular
    if (diffuseCoefficient != .0f)
    {
        vec3 incidentDirection = -surfaceToLightDirection;
        vec3 reflectionDirection = reflect(incidentDirection, surfaceNormal);
        vec3 surfaceToEye = normalize(viewBlock.eyePosition - vertexOutput.position);
        float cosAngle = max(.0f, dot(surfaceToEye, reflectionDirection));
        float specularCoefficient = pow(cosAngle, materialSpecularBlock.shininess);
        specularIntensities += specularCoefficient * materialSpecularBlock.color * pointLightIntensities;
    }

    float attenuationDenominator = light.constantAttenuationFactor +
                                   light.linearAttenuationFactor * distanceSurfaceToLight +
                                   light.quadraticAttenuationFactor * pow(distanceSurfaceToLight, 2.0);

    return (diffuseIntensities + specularIntensities) / attenuationDenominator;
}

vec3 DirectionalLightColor(vec3 surfaceNormal, DirectionalLight light)
{

    float cosAoiAngle = dot(surfaceNormal, -light.direction.xyz);
    float diffuseCoefficient = max(0.0f, cosAoiAngle);

    return diffuseCoefficient * light.intensities.rgb * materialDiffuseBlock.color;
}


void main(void)
{
    vec3 normal = normalize(vertexOutput.normal);

    vec3 linearColor = ambient;

    for (int directionalLightIdx = 0; directionalLightIdx < DirectionalLightBlock.numDirectionalLights; ++directionalLightIdx)
    {
        DirectionalLight directionalLight = DirectionalLightBlock.directionalLights[directionalLightIdx];
        linearColor += DirectionalLightColor(normal, directionalLight);
    }

    for (int pointLightIdx = 0; pointLightIdx < PointLightBlock.numPointLights; ++pointLightIdx)
    {
        PointLight pointLight = PointLightBlock.pointLights[pointLightIdx];
        linearColor += PointLightColor(normal, pointLight);
    }

    vec3 gamma = vec3(1.0f / 2.2f);
    color = vec4(pow(linearColor, gamma), 1.0f);
}
