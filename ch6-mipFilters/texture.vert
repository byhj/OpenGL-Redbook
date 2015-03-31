#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_tex_coord;

out vec2 tex_coord;
uniform mat4 tc_rotate;

void main(void)
{
    gl_Position = tc_rotate * vec4(in_position, 1.0);
    tex_coord = in_tex_coord;
}