#version 430 core

layout (location = 0) in vec4 Position;

void main(void)
{
  gl_Position = Position;
}