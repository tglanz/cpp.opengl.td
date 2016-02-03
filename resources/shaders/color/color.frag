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
    // todo: refactor to uniform struct array
    vec3 light_pos = vec3(0.0f, 0.0f, 5.0f);
    vec3 light_intensities = vec3(.2f, 0.2f, 0.2f);
    float attenuation_factor = .0f;

    vec3 surface_to_light = light_pos - vs_position;
    vec3 surface_to_light_direction = normalize(surface_to_light);
    float cos_aoi_angle = dot(vs_normal, surface_to_light_direction);

    float distance_to_light = length(surface_to_light);
    float attenuation = 1.0f / (1.0f + attenuation_factor * pow(distance_to_light, 2.0f));

    // diffuse
    float diffuse_coefficient = max(0.0f, cos_aoi_angle);
    vec3 diffuse_intensities = diffuse_coefficient * vs_color.rgb * light_intensities;

    // specular
    float specular_coefficient = .0f;
    if (diffuse_coefficient != .0f)
    {
        vec3 incident_direction = -surface_to_light_direction;
        vec3 reflection_direction = reflect(incident_direction, vs_normal);
        vec3 surface_to_eye = normalize(eye_position - vs_position);
        float cos_angle = max(.0f, dot(surface_to_eye, reflection_direction));
        specular_coefficient = pow(cos_angle, material_specular_shininess);
    }
    vec3 specular_intensities = specular_coefficient * material_specular_color * light_intensities;

    vec3 linear_color = ambient + attenuation * (diffuse_intensities + specular_intensities);

    vec3 gamma = vec3(1.0f / 2.2f);
    color = vec4(pow(linear_color, gamma), 1.0f);
}
