#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

out VS_OUT {
  vec3 normal;
}vs_out;

void main()
{
   vs_out.normal = mat3(model_matrix) * normal;
   gl_Position = mvp_matrix * position;
}