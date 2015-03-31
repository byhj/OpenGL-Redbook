#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT {
  vec3 normal;
  vec4 color;
} fs_in;

void main()
{
	fragColor = fs_in.color * (0.1 + abs(fs_in.normal.z)) + 
	            vec4(0.8, 0.9, 0.7, 1.0) * pow(abs(fs_in.normal.z), 40.0) ;
			 // π”√¡Àspecular heightlight
}
