 #version 430 core

 layout (location = 0) in vec4 position; //实例顶点
 layout (location = 1) in vec3 normal; //实例法线
 layout (location = 2) in vec4 color; //实例颜色
 layout (location = 3) in mat4 model_matrix; //实例模型矩阵

 uniform mat4 view_matrix;
 uniform mat4 proj_matrix;

out VS_OUT {
   vec3  normal; //法线
   vec4  color;  //颜色
} vs_out;

void main(void)
{
    mat4 model_view_matrix = view_matrix * model_matrix;
    gl_Position = proj_matrix * (model_view_matrix * position); //mvp matrix
	vs_out.normal = mat3(model_view_matrix) * normal; //平移不改变法线，我们只使用左上角的3x3矩阵
	vs_out.color = color;
}