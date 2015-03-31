#version 430 core

uniform sampler2D sprite_texture;

out vec4 fragColor;

void main(void)
{
   //获取纹理数据，纹理坐标基于点片元的坐标
    fragColor = texture(sprite_texture, vec2(gl_PointCoord.x, 1.0 - gl_PointCoord.y));
}
