#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT {
   vec2 tc;
}fs_in;

uniform sampler2D tex;

void main(void)
{
    fragColor = texture(tex, fs_in.tc); //获取纹理，使用sampler和纹理坐标
}