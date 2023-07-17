#version 430

layout(std430, binding = 0) buffer buf
{
    mat4 mat[];
}jointMatrices;

in vec3 position;
in vec3 normal;
in vec2 uv;
in vec4 joints;
in vec4 weights;

out vec4 color;

uniform mat4 M;
uniform mat4 VP;    //P * V

void main()
{
    mat4 skinMat =
        weights.x * jointMatrices.mat[int(joints.x)] +
        weights.y * jointMatrices.mat[int(joints.y)] +
        weights.z * jointMatrices.mat[int(joints.z)] +
        weights.w * jointMatrices.mat[int(joints.w)];
    vec4 worldPosition = skinMat * vec4(position,1.0);
    gl_Position = VP * worldPosition;
    color = vec4(position, 1.0f);
}