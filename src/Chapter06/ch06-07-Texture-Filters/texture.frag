#version 430 core

layout (location = 0) out vec4 fragColor;

uniform sampler2D tex;

in vec2 tc;

void main(void)
{
    fragColor = texture(tex, tc); 
}