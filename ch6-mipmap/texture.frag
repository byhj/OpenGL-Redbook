#version 430 core

layout (location = 0) out vec4 fragColor;

in vec2 tex_coord;
uniform sampler2D gSampler;

void main(void)
{
    fragColor = texture(gSampler, tex_coord); //获取纹理，使用sampler和纹理坐标
}