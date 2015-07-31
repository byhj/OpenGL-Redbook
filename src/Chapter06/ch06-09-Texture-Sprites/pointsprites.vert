#version 430 core

layout (location = 0) in vec4 position;

uniform mat4 model;
uniform mat4 proj;

void main(void)
{
    vec4 pos = (proj * model) * position;
    gl_PointSize = (1.0f - pos.z / pos.w) * 64.0f;
    gl_Position = pos;
}
