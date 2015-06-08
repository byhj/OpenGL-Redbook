#version 430 core

layout (location = 0) in vec2 Position;

void main(void)
{

   gl_Position = vec4(Position, 0.0f, 1.0f);
}