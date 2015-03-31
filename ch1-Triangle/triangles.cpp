#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>

using namespace std;

#include <shader.h>

enum VAO_IDs { Triangles, NumVAOs };  //VAO索引
enum Buffer_IDs { ArrayBuffer, NumBuffers }; //VBO索引
enum Attrib_IDs { vPosition = 0 };  //顶点属性

GLuint VAOs[NumVAOs];   //顶点属性数组
GLuint Buffers[NumBuffers];  //缓存对象
const GLuint NumVertices = 6; //顶点数
Shader triangleShader("triangle shader");  //shader对象
const int Width = 800, Height = 640; //窗口宽高

void load_shaders()
{
	triangleShader.init();
	triangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	triangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	triangleShader.link();
	triangleShader.use();
}

void init(void)
{
	load_shaders();

	glClearColor(0.0, 0.0, 0.0, 0.0); //设置颜色缓存为黑色

	glGenVertexArrays(NumVAOs, VAOs);  //创建VAO管理状态
	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[NumVertices][2] = {  //顶点数据
		{ -0.90f, -0.90f }, // Triangle 1
		{ 0.85f, -0.90f },
	    { -0.90f, 0.85f },

		{ 0.90f, -0.85f }, // Triangle 2
		{ 0.90f, 0.90f },
		{ -0.85f, 0.90f }
		};

	glGenBuffers(NumBuffers, Buffers); //产生VBO，将顶点数据传入
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//指定顶点属性，shader的数据接口
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //视口变换

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓存为前面设置值

	glBindVertexArray(VAOs[0]);  //使用这个VAO管理的相关状态绘制
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);  //绘制三角形

	glutSwapBuffers();  //强制执行命令，刷新
	//使用glFinish才等待全部命令完成
}


int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	////glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Triangle");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init(); //初始化资源
	glutDisplayFunc(display);  //设置循环调用显示函数
	glutReshapeFunc(reshape);  //视口变化函数
	glutMainLoop();    //执行循环
	return 0;
}


