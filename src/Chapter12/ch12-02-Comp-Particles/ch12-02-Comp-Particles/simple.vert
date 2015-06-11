#version 430 core

layout (location = 0) in vec4 Position;

uniform mat4 mvp;
out float intensity;

void main(void)
{
  intensity = Position.w;
  gl_Position = mvp * vec4(Position.xyz, 1.0f);
}