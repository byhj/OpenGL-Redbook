#version 430 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec4 color;

//mat4 occupy four vertex Attrib
layout (location = 3) in mat4 model;

uniform mat4 view;
uniform mat4 proj;

out VS_OUT
{
   vec3 normal;
   vec4 color;
}vs_out;


void main(void)
{

    mat4 mv = view * model;
	mat4 mvp = proj * mv;

	vs_out.normal = normalize( mat3(mv) * Normal);
	vs_out.color = color;

	gl_Position = mvp * Position;
}
