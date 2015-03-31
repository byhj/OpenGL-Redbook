#version 430 core

in VS_OUT {
  vec4 color;
}fs_in;
 
layout (location = 0) out vec4 fragColor;

void main(void)
{
	fragColor = fs_in.color;
}
