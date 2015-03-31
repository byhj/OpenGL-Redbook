#include <GL/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
#include <shader.h>

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
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    static const GLfloat quad_data[] = {
         0.75f, -0.75f,  //顶点坐标
        -0.75f, -0.75f,
        -0.75f, 0.75f,
         0.75f, 0.75f,

         0.0f, 0.0f, //纹理坐标
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

	glUseProgram(base_prog);

	glGenTextures(1, &tex);  //请注意纹理对象产生和绑定后缀！！！
	glBindTexture(GL_TEXTURE_2D, tex); //创建纹理对象，将纹理数据传入
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, 8, 8);
	//创建纹理缓存区
	static const GLubyte tex_checkboard_data[] = { //棋盘的黑白纹理
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
    };
	//真正传入纹理数据到纹理对象
	glTexSubImage2D(GL_TEXTURE_2D, 0, 
		          0, 0,   //x, y的offset
				  8, 8,   //宽度和高度
				  GL_RED, GL_UNSIGNED_BYTE,
				  tex_checkboard_data);  //纹理数据

    static const GLint swizzles[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
	//设置纹理过滤和纹理环绕模式
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);

}

void reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
    aspect = float(width) / float(height);
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

	//清除颜色和深度缓存
    glUseProgram(base_prog);
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	////glutInitContextVersion(4, 3);
	////glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Texture");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}


