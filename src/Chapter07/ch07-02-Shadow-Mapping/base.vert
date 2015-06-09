#version 430 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 shadow_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out VS_FS_INTERFACE
{
    vec4 shadow_coord;
    vec3 world_coord;
    vec3 eye_coord;
    vec3 normal;
}vs_out;

void main(void)
{
    vec4 world_pos = model * position;
    vec4 eye_pos   = view * world_pos;
    vec4 clip_pos  = proj * eye_pos;

    vs_out.world_coord = world_pos.xyz;
    vs_out.eye_coord = eye_pos.xyz;
    vs_out.shadow_coord = shadow_matrix * world_pos;
    vs_out.normal = mat3(view * model) * normal;

    gl_Position = proj * view * model * position;
}
