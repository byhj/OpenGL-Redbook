#version 430 core

layout (location = 0) out vec4 fragColor;

uniform samplerCube	tex;
in vec3 vNormal;
in vec3 vPosition;

void main(void)
{
    vec3 tc =  reflect(vPosition, normalize(vNormal));
    fragColor = vec4(0.3, 0.2, 0.1, 1.0) + vec4(0.97, 0.83, 0.79, 0.0) * texture(tex, tc);
}
 