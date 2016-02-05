/*#version 430 core

uniform vec3 ambient;

in vec4 vs_color;
in vec3 vs_position;
in vec3 vs_normal;


out vec4 color;

void main(void)
{
    vec3 light_pos = vec3(0.0f, 0.0f, 5.0f);
    vec3 light_intensities = vec3(1.0f, 0.0f, 0.0f);

    // L
    vec3 pos_to_light = light_pos - vs_position;
    float attenuation = 1.0 / pow(length(pos_to_light), 2.0f);
    float brightness = dot(vs_normal, pos_to_light) * attenuation;
    brightness = clamp(brightness, 0.0f, 1.0f);

    color = vs_color;
    color = vec4(brightness * light_intensities, 0);
}

*/
#version 430 core

uniform vec3 eye_position;
uniform vec3 ambient;

uniform vec3 material_specular_color;
uniform float material_specular_shininess;

in vec4 vs_color;
in vec3 vs_position;
in vec3 vs_normal;

out vec4 color;

void main(void)
{
    vec3 normal = normalize(vs_normal);

    // todo: refactor to uniform struct array
    vec3 light_pos = vec3(0.0f, 0.0f, 5.0f);
    vec3 light_intensities = vec3(1.0f, 1.0f, 1.0f);
    float attenuation_factor = .0f;

    vec3 surfaceToLight = light_pos - vs_position;
    vec3 surfaceToLightDirection = normalize(surfaceToLight);
    float cosAoiAngle = dot(normal, surfaceToLightDirection);

    float distanceSurfaceToLight = length(surfaceToLight);
    float attenuation = 1.0f / (1.0f + attenuation_factor * pow(distanceSurfaceToLight, 2.0f));

    // diffuse
    float diffuseCoefficient = max(0.0f, cosAoiAngle);
    vec3 diffuseIntensities = diffuseCoefficient * light_intensities;

    // specular
    float specularCoefficient = .0f;
    if (diffuseCoefficient != .0f)
    {
        vec3 incidentDirection = -surfaceToLightDirection;
        vec3 reflectionDirection = reflect(incidentDirection, normal);
        vec3 surfaceToEye = normalize(eye_position - vs_position);
        float cosAngle = max(.0f, dot(surfaceToEye, reflectionDirection));
        specularCoefficient = pow(cosAngle, material_specular_shininess);
    }
    vec3 specularIntensities = specularCoefficient * material_specular_color * light_intensities;

    vec3 linearColor = ambient + attenuation * (diffuseIntensities + specularIntensities) * vs_color;

    vec3 gamma = vec3(1.0f / 2.2f);
    color = vec4(pow(linearColor, gamma), 1.0f);
}
