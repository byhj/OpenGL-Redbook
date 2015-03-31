#version 430 core

layout (location = 0) out vec4 fragColor;

in vec2 tex_coord0; //纹理坐标
in vec2 tex_coord1;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main(void)
{
    fragColor =  texture(tex0, tex_coord0) + texture(tex1, tex_coord1); 
	                       //获取纹理，使用sampler和纹理坐标
    
}