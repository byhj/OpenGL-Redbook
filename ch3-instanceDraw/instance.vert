 #version 430 core

 layout (location = 0) in vec4 position; //由模型给出
 layout (location = 1) in vec3 normal;

 layout (location = 3) in vec4 weights;
 layout (location = 4) in vec4 color;
 
 uniform mat4 model_matrix[4];
 uniform mat4 proj_matrix;

 out VS_OUT {
    vec3 normal;
    vec4 color;
}vs_out;


void main(void)
{
      mat4 m = mat4(0.0f); //0矩阵？
      vec4 weights = normalize(weights);

      for (int n = 0; n < 4; n++) 
	     m += (model_matrix[n] * weights[n]);

  // Calulate a weighted average of the matrices
  // Use that calculated matrix to transform the object.

  vs_out.normal = normalize((m * vec4(normal, 0.0)).xyz);
  vs_out.color = color;
  gl_Position = proj_matrix * (m * position);

}