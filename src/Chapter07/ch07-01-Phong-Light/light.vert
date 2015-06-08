#version 430 core

uniform mat4 model;
uniform mat4 proj;


layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;

out VS_OUT
{ 
  vec3 worldPos;
  vec3 normal;
}vs_out;

void main(void)
{
   
   vec4 pos = (proj  * model) * Position;
   vs_out.worldPos = pos.xyz;
   vs_out.normal = mat3(model) * Normal;

   gl_Position = pos;
}