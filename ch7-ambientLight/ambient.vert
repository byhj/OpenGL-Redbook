#version 430 core

layout (location = 0) in vec4 position;

uniform mat4 mvp_matrix;

void main()
{
   gl_Position = mvp_matrix * position;
}