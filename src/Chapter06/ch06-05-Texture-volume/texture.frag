#version 430 core

layout (location = 0) out vec4 fragColor;

in vec3 tc;
uniform sampler3D tex;

uniform vec4 randomColor;

void main(void)
{
	fragColor = texture(tex, tc).rrrr * randomColor;
}
