 #version 430 core

 layout (location = 0) in vec2 Position;
 layout (location = 1) in vec2 TexCoord;

 out vec3 tc;

 uniform mat4 tc_rotate;

 void main(void)
 {
    gl_Position =  vec4(Position, 0.5f, 1.0f);
    tc = (vec4(TexCoord, 0.0, 1.0) * tc_rotate ).stp;
}