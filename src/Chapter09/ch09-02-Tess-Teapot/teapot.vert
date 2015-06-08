#version 430 core

in layout(location = 0) vec4 vPosition;

void main()
{
    gl_Position = vPosition;
}
