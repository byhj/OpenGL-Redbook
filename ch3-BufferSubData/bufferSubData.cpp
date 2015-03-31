#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>

using namespace std;

#include "shader.h""

enum VAO_IDs { Triangles, NumVAOs };  //VAO
enum Buffer_IDs { ArrayBuffer, NumBuffers }; //VBO
enum Attrib_IDs { vPosition = 0, vColor = 1 };  //顶点属性

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 3;
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


static const GLfloat positions[] = {  //顶点
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
};

static const GLfloat colors[] = {  //颜色
	1.0f, 0.0f, 0.0f, 1.0f, // red
	0.0f, 1.0f, 0.0f, 1.0f,//green
	0.0f, 0.0f, 1.0f, 1.0f//blue                                                       //white
};

void init(void)
{
	load_shaders();
	glClearColor(0.0, 0.0, 0.0, 0.0); //背景为黑色
	glGenVertexArrays(NumVAOs, VAOs);  //创建VAO管理状态
	glBindVertexArray(VAOs[Triangles]);

	glGenBuffers(NumBuffers, Buffers); //创建VBO，将顶点数据传入
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors),
		         NULL, GL_STATIC_DRAW);//先分配内存，不传入数据

    //使用sub函数将顶点和颜色数据传入到同一个VBO中
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);
	//颜色数据存入时，给出当前vbo的位置，即顶点数据后

    //VBO数据传入shader，注意顶点和颜色在同一vbo中，要给出它们的位移
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(positions));
    glEnableVertexAttribArray(vColor); //颜色数据在顶点数据后面

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓存
	glBindVertexArray(VAOs[0]);  //使用这个VAO管理的相关状态绘制
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);  //绘制三角形

	glFlush();  //强制执行命令，刷新
}


int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("BufferSubData");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
