#version 430

in VS_OUT 
{
  vec2 tc;
  noperspective vec2 tc_np;
} fs_in;

uniform bool use_perspective = true;
uniform sampler2D tex_checker;

layout (location = 0) out vec4 fragColor;

void main()
{    
    //mix: x(1-a) + y*a
     vec2 tc = mix(fs_in.tc_np, fs_in.tc, bvec2(use_perspective));  
     fragColor = texture2D(tex_checker, tc).rrrr;  
}