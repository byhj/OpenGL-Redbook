#version 430 core

layout (location = 0) out vec4 fragColor;

unifrom vec4 ambient;

in VS_OUT {
  vec4 color;
}fs_in;

void main()
{
    fragColor = min(fs_in.color * ambeint, vec4(1.0));
}