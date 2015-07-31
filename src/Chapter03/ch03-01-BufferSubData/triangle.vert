 #version 430 core

 layout (location = 0) in vec4 vPosition;
 layout (location = 1) in vec4 vColor;

 out VS_OUT
 {
   vec4 color;
 }vs_out;

 void main()
 {
	vs_out.color = vColor;
    gl_Position = vPosition;
}