#version 430 core

layout (location = 0) out vec4 fragColor;

uniform vec4 ambient;

void main()
{
    fragColor = min( ambient, vec4(1.0));
}