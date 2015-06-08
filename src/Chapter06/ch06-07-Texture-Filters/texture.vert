#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out vec2 tc;
uniform mat4 tc_rotate;

void main(void)
{
    gl_Position = tc_rotate * vec4(Position, 1.0);
    tc = TexCoord;
}