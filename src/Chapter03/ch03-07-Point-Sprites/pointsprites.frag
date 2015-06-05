#version 430 core

uniform sampler2D sprite_texture;

out vec4 fragColor;

void main(void)
{
	vec2 tc = gl_PointCoord.xy;
    fragColor = texture(sprite_texture, vec2(gl_PointCoord.x, 1.0 - gl_PointCoord.y));

}
