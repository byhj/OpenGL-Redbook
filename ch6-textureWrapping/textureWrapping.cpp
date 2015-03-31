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
	
    static const GLfloat quad_data[] =
    {
         0.75f, -0.75f,
        -0.75f, -0.75f,
        -0.75f, 0.75f,
         0.75f, 0.75f,

         0.0f, 0.0f, //超出[0，1]部分按指定环绕模式绘制
         4.0f, 0.0f,
         4.0f, 4.0f,
         0.0f, 4.0f
    };

	glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, 8, 8);

    static const unsigned char texture_data[] =
    {
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
    };

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0, 0,
                    8, 8,
                    GL_RED, GL_UNSIGNED_BYTE,
                    texture_data);

    static const GLint swizzles[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
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
	unsigned int ticks = GetTickCount(); //鼠标点击次数
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
	//清除颜色和深度缓存
    glUseProgram(base_prog);
	//设置纹理环绕模式，边插值，区域插值，重复，镜像
    static const GLenum wrap_modes[] = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRRORED_REPEAT };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_modes[(ticks >> 11) & 0x3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_modes[(ticks >> 11) & 0x3]);
    
	glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Texture");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


