#version 430 core

uniform sampler2DShadow depth_texture;

uniform vec3  lightPos;
uniform vec3  mat_ambient;
uniform vec3  mat_diffuse;
uniform vec3  mat_specular;
uniform float mat_specular_power;

layout (location = 0) out vec4 fragColor;

in VS_FS_INTERFACE
{
    vec4 shadow_coord;
    vec3 world_coord;
    vec3 eye_coord;
    vec3 normal;
}fs_in;

void main(void)
{
    vec3 N = fs_in.normal;
    vec3 L = normalize(lightPos - fs_in.world_coord);
    float LdotN = dot(N, L);
    vec3 R = reflect(-L, N);

    float diffuse = max(LdotN, 0.0);
    float specular = max(pow(dot(normalize(-fs_in.eye_coord), R), mat_specular_power), 0.0);

	//proj the shadow texture
    float f = textureProj(depth_texture, fs_in.shadow_coord);

    fragColor = vec4(mat_ambient + f * (mat_diffuse * diffuse + mat_specular * specular), 1.0);
}
