#version 430 core

layout (location = 0) out vec4 fragColor;

in float intensity;

void main(void)
{
  fragColor = mix(vec4(0.0f, 0.2f, 1.0f, 1.0f), vec4(0.2f, 0.05f, 0.0f, 1.0f), intensity);
}