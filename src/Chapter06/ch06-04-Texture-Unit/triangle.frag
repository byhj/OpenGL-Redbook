#version 430 core

layout (location = 0) out vec4 fragColor;

in vec2 tc;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main(void)
{
	fragColor = texture(tex0, tc) + texture(tex1, tc);
}
