#version 430 core

layout (location = 0) in vec4 position;

unifrom mat4 mvp_matrix;

out VS_OUT {
  vec4 color;
}vs_out;

void main()
{
   vs_out.color = color;
   gl_Position = mvp_matrix * position;
}