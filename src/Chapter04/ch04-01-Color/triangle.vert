 #version 430 core

 layout (location = 0) in vec2 vPosition;
 layout (location = 1) in vec4 vColor;

 out vec4 color;

 void main(void)
 {
    gl_Position =  vec4(vPosition, 0.0f, 1.0f);
    color = vColor;
}