#version 430 core

uniform sampler2D sprite_texture;

out vec4 fragColor;

void main(void)
{
   /*
    const vec4 color1 = vec4(0.6, 0.0, 0.0, 1.0);
	const vec4 color2 = vec4(0.9, 0.7, 1.0, 0.0);
	vec2 temp = gl_PointCoord - vec2(0.5);
	float f = dot (temp, temp);
	if (f > 0.25)
	   discard;
	fragColor = mix (color1, color2, smoothstep(0.1, 0.25, f) );
	*/
	vec2 tc = gl_PointCoord.xy;
    fragColor = texture(sprite_texture, vec2(gl_PointCoord.x, 1.0 - gl_PointCoord.y));

}
