#version 430 core

layout (location = 0) out vec4 fragColor;

uniform samplerCube tex;
in vec3 tc;

void main(void)
{
   fragColor = texture(tex, tc);
}