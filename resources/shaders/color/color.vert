#version 430 core

layout(std140) uniform ViewBlock
{
    mat4 viewProjection;
    mat4 perspectiveProjection;
    vec3 eyePosition;
} viewBlock;

uniform mat4 model_projection;

in vec3 in_position;
in vec3 in_normal;

out VertexOutput
{
    vec3 position;
    vec3 normal;


} vertexOutput;

void main(void)
{
    mat3 normal_projection = transpose(inverse(mat3(model_projection)));

    vec4 modelPosition = model_projection * vec4(in_position, 1.0f);

    vertexOutput.position = modelPosition.xyz;
    vertexOutput.normal = normalize(normal_projection * in_normal);

    gl_Position = viewBlock.perspectiveProjection * viewBlock.viewProjection * modelPosition;
}
