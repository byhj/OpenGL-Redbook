 #version 430 core

 layout (location = 0) in vec4 vPosition;
 layout (location = 1) in vec4 vColor;

 uniform mat4 model_matrix;
 uniform mat4 mvp_matrix;

out VS_OUT {
  vec4 color;
}vs_out;


 void main(void)
 {
    gl_Position =  mvp_matrix * (model_matrix* vPosition);
    vs_out.color = vColor;
}