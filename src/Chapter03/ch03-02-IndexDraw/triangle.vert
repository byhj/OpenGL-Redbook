 #version 430 core

 layout (location = 0) in vec4 vPosition;
 layout (location = 1) in vec4 vColor;

 out vec4 color;

 uniform mat4 mvp;

 void main()
 {
    gl_Position = mvp *  vPosition;
	color = vColor;
}