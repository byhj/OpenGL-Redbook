#version 430 core

in VERTEX {
   vec3  normal;
   vec4  color;
} vertex;

out vec4 fragColor;

void main()
{
  fragColor = vertex.color * (0.1 + abs(vertex.normal.z)) +
              vec4(0.8, 0.9, 0.7, 1.0) * pow(abs(vertex.normal.z), 40.0);
}
