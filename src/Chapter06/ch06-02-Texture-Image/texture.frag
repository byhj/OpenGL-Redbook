#version 430 core

layout (location = 0) out vec4 fragColor;

in vec2 tc;
uniform sampler2D tex;

void main(void)
{
	fragColor = texture(tex, tc);
}
