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

// Member variables
static float aspect;
GLuint vao[1];
GLuint vbo[1];
GLuint program;
GLint  mvpLoc, diffuseLoc, dirctionLoc, ambientLoc;
VBObject object;
Shader diffuseShader("instance Shader");

void init_shaders()
{
	diffuseShader.init();
	diffuseShader.attach(GL_VERTEX_SHADER, "ambient.vert");
	diffuseShader.attach(GL_FRAGMENT_SHADER, "ambient.frag");
	diffuseShader.link();
	diffuseShader.use();
	program = diffuseShader.program;  //获取矩阵的uniform位置
	mvpLoc = glGetUniformLocation(program, "mvp_matrix");
	diffuseLoc = glGetUniformLocation(program, "ambient");

}

void init(void)
{
	//glEnable(GL_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 0.0); //背景为黑色
	init_shaders();
	//加载模型
	object.LoadFromVBM("../media/objects/bunny.vbm", 0, 1, 2); //顶点，索引，纹理
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	aspect = float(w) / float(h);

}

void display(void)
{
	float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);
	//每次鼠标点击会改变绘制

	static float q = 0.0f;
	static const glm::vec3 X(1.0f, 0.0f, 0.0f);
	static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
	static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set four model matrices
	glm::vec4 ambient = glm::vec4(1.0, 0.0, 1.0, 0.0);
	glm::mat4 mv_matrix = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 proj_matrix = glm::perspective(38.0f, 1.0f, 0.1f, 1000.0f);
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
	glUniform4fv(diffuseLoc, 1, glm::value_ptr(ambient));
	object.Render();	//使用实例绘制模型
	glutSwapBuffers();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Instance Draw");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK)
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


