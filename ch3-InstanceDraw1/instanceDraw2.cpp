#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace std;

#include <shader.h>
#include <vbm.cpp>

const int Width = 800, Height = 640; //窗口宽高
const int  INSTANCE_COUNT = 100 ; //绘制实例数
    // Member variables

float aspect;
GLuint color_buffer;
GLuint model_matrix_buffer;
GLuint render_prog;
GLint model_matrix_loc;
GLint view_matrix_loc;
GLint proj_matrix_loc;
VBObject object;
Shader instanceShader("instance Shader");

void load_shaders()
{
	instanceShader.init();
	instanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	instanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	instanceShader.link();
	instanceShader.use();
	render_prog = instanceShader.program;  //获取矩阵的uniform位置
	view_matrix_loc = glGetUniformLocation(render_prog, "view_matrix");
	proj_matrix_loc = glGetUniformLocation(render_prog, "proj_matrix");
	int position_loc = glGetAttribLocation(render_prog, "position");
	int normal_loc = glGetAttribLocation(render_prog, "normal");
	int color_loc = glGetAttribLocation(render_prog, "color");
	int matrix_loc = glGetAttribLocation(render_prog, "model_matrix");

}

void init(void)
{
	//glEnable(GL_DEPTH);
	load_shaders();
	glClearColor(0.0, 0.0, 0.0, 0.0); //背景为黑色
    glEnable(GL_CULL_FACE);  //背面剔除
    glDisable(GL_DEPTH_TEST); //不启用深度测试
	//获取顶点属性索引0， 1， 2，matrix占据3， 4， 5 ，6
    int position_loc = glGetAttribLocation(render_prog, "position");
    int normal_loc   = glGetAttribLocation(render_prog, "normal");
    int color_loc    = glGetAttribLocation(render_prog, "color");
    int matrix_loc   = glGetAttribLocation(render_prog, "model_matrix");

	// Load the object
    object.LoadFromVBM("../media/objects/armadillo_low.vbm", 0, 1, 2);
    // Bind its vertex array object so that we can append the instanced attributes
    object.BindVertexArray();

    glm::vec4 colors[INSTANCE_COUNT]; //实例模型颜色
    for (int n = 0; n < INSTANCE_COUNT; n++) {
        float a = float(n) / 4.0f;
        float b = float(n) / 5.0f;
        float c = float(n) / 6.0f;

        colors[n][0] = 0.5f + 0.25f * (sinf(a + 1.0f) + 1.0f);
        colors[n][1] = 0.5f + 0.25f * (sinf(b + 2.0f) + 1.0f);
        colors[n][2] = 0.5f + 0.25f * (sinf(c + 3.0f) + 1.0f);
        colors[n][3] = 1.0f;
    }

    glGenBuffers(1, &color_buffer);  //将颜色数据传入到顶点缓存对象
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);

    // Now we set up the color array. We want each instance of our geometry
    // to assume a different color, so we'll just pack colors into a buffer
    // object and make an instanced vertex attribute out of it.
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	//将颜色数据传入到shader
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_loc);
    // This is the important bit... set the divisor for the color array to
    // 1 to get OpenGL to give us a new value of 'color' per-instance
    // rather than per-vertex.
    glVertexAttribDivisor(color_loc, 1); //每个实例使用不同的颜色绘制，而不是每个顶点

    // Likewise, we can do the same with the model matrix. Note that a
    // matrix input to the vertex shader consumes N consecutive input
    // locations, where N is the number of columns in the matrix. So...
    // we have four vertex attributes to set up.
	//将模型矩阵数组传入
    glGenBuffers(1, &model_matrix_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, model_matrix_buffer);
    glBufferData(GL_ARRAY_BUFFER, INSTANCE_COUNT * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    // Loop over each column of the matrix...
    for (int i = 0; i < 4; i++) {
        // Set up the vertex attribute //占据4个顶点属性索引
        glVertexAttribPointer(matrix_loc + i,              // Location
                              4, GL_FLOAT, GL_FALSE,       // vec4
                              sizeof(glm::mat4),                // Stride
                              (void *)(sizeof(glm::vec4) * i)); // Start offset
        // Enable it
        glEnableVertexAttribArray(matrix_loc + i);
        // Make it instanced
        glVertexAttribDivisor(matrix_loc + i, 1); //每个实例使用4个矩阵
    }

    // Done (unbind the object's VAO)
    glBindVertexArray(0);

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	aspect = float(h) / float(w);

}

void display(void)
{
	 // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

   float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);
   static float q = 0.0f;
   static const glm::vec3 X(1.0f, 0.0f, 0.0f);
   static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
   static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, model_matrix_buffer);
    // Set model matrices for each instance
	//使用指针映射方式获取模型矩阵的数据
    glm::mat4 * matrices = (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	//mapBuffer使用指针获取数据，不用复制
    for (int n = 0; n < INSTANCE_COUNT; n++) {
        float a = 50.0f * float(n) / 4.0f;
        float b = 50.0f * float(n) / 5.0f;
        float c = 50.0f * float(n) / 6.0f;
		//指定模型变换矩阵
        matrices[n] = glm::rotate(glm::mat4(1.0), a + t * 360.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0),b + t * 360.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0),c + t * 360.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
                      glm::translate(glm::mat4(1.0),glm::vec3(10.0f + a, 40.0f + b, 50.0f + c));
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    //设置投影和视图矩阵
    glm::mat4 projection_matrix(glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) );           
	glUniformMatrix4fv(proj_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glm::mat4 view_matrix(glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1500.0f)) 
		                * glm::rotate(glm::mat4(1.0), t * 360.0f * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) );
	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	// Render INSTANCE_COUNT objects
    object.Render(0, INSTANCE_COUNT);	

	glutSwapBuffers();
	//glutPostRedisplay();
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Triangle");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


