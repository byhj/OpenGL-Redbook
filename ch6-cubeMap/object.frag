#version 330 core

layout (location = 0) out vec4 color;

uniform samplerCube tex;
in vec3 vs_fs_normal;
in vec3 vs_fs_position;

void main(void)
{
    vec3 tc =  reflect(vs_fs_position, normalize(vs_fs_normal));
    color = vec4(0.3, 0.2, 0.1, 1.0) + vec4(0.97, 0.83, 0.79, 0.0) * texture(tex, tc);
}