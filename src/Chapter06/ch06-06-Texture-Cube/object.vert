#version 430 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;

out vec3 vNormal;
out vec3 vPosition;

uniform mat4 mv;
uniform mat4 proj;

void main(void)
{
   mat4 mvp = proj * mv;
   vNormal = mat3(mv) * Normal;
   vPosition = (mv * Position).xyz;
   gl_Position = mvp * Position;  
}
