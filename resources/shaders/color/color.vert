#version 430 core

uniform mat4 model_projection;
uniform mat4 view_projection;
uniform mat4 perspective_projection;
uniform vec3 eye_position;

uniform vec3 ambient;

uniform vec3 material_specular_color;
uniform float material_specular_shininess;

in vec3 in_position;
in vec4 in_color;
in vec3 in_normal;

out vec4 vs_color;
out vec3 vs_position;
out vec3 vs_normal;

void main(void)
{
    mat3 normal_projection = transpose(inverse(mat3(model_projection)));
    vs_normal = normalize(normal_projection * in_normal);

    vs_color = in_color; // pass the color to the fragment shader
    vs_position = in_position; // pass the position to the fragment shader

    gl_Position = perspective_projection * view_projection * model_projection * vec4(in_position, 1.0f);
}
