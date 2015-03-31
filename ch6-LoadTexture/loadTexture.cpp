#include <GL/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include <shader.h>
#include <loadTexture.h>

float aspect;
GLuint base_prog;
GLuint vao;
GLuint quad_vbo;
GLuint tex, gSampler;
Shader textureShader("Texture shader");
const int Width = 800, Height = 640; //窗口宽高

void load_shaders()
{
	textureShader.init();
	textureShader.attach(GL_VERTEX_SHADER, "texture.vert");
	textureShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	textureShader.link();
	textureShader.use();
	base_prog = textureShader.program;
    gSampler = glGetUniformLocation(base_prog, "gSampler");  //sampler是shader获取纹理数据的方法
    glUniform1i(gSampler, 0); //指定采样器索引为0；

}

void init(void)
{
	load_shaders();
	tex = loadTexture("../media/textures/test.dds");
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static const GLfloat quad_data[] =  {
         0.8f, -0.8f,  //正方形数据 ,逆时针绘制
        -0.8f, -0.8f,
        -0.8f, 0.8f,
         0.8f, 0.8f,

         0.0f, 0.0f,  //纹理坐标数据，要怎么映射到物体上，左下角为（0,0）
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f
    };

	glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
	//将顶点和纹理坐标数据传到shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
    aspect = float(width) / float(height);
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//清除颜色和深度缓存

	glBindTexture(GL_TEXTURE0, tex); //绑定想要使用的纹理
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //绘制四边形
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Load Texture");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


