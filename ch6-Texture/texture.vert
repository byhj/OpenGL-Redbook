#version 430 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out VS_OUT  {
  vec2 tc;
}vs_out;

void main(void)
{
    gl_Position = vec4(position, 0.5, 1.0);
    vs_out.tc = texCoord;
}