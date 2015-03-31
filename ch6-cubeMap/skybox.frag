#version 330 core

layout (location = 0) out vec4 fragColor;

uniform samplerCube tex;
in vec3 tex_coord;

void main()
{
    fragColor = texture(tex, tex_coord);
}