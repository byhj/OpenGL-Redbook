#version 430 core

layout (location = 0) out vec4 fragColor;

uniform vec4 ambient = vec4(1.0);
vec3 direction = vec3(0.5, -0.3, 0.0);

in VS_OUT {
  vec3 normal;
}fs_in;

void main()
{
    vec3 n = normalize(fs_in.normal);
	float diffuse = dot(n, direction);
    fragColor = min(diffuse * vec4(1.0) * 0.7 + vec4(1.0) * 0.5, vec4(1.0));
}