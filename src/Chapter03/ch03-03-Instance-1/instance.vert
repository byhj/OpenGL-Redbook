#version 430 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;

layout (location = 3) in vec4 weights;
layout (location = 4) in vec4 color;

uniform mat4 model[4];
uniform mat4 proj;

out VS_OUT
{
   vec3 normal;
   vec4 color;
}vs_out;


void main(void)
{

    mat4 m = mat4(0.0f);
	vec4 weights = normalize(weights);

	for (int n = 0; n != 4; ++n)
       m += (model[n] * weights[n]);
	
	vs_out.normal = normalize( mat3(m) * Normal);
	vs_out.color = color;

	gl_Position = proj * (m * Position);
}
