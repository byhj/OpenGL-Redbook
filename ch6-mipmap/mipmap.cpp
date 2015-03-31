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
GLuint vao, skybox_rotate_loc;
GLuint cube_vbo, cube_element_buffer;
GLuint texture, texture_color;
GLuint gSampler;
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
    skybox_rotate_loc = glGetUniformLocation(base_prog, "tc_rotate");

}

void init(void)
{
	load_shaders();
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
	
    static const GLfloat plane_vertices[] =
    {
        -20.0f, 0.0f, -50.0f,
        -20.0f, 0.0f,  50.0f,
         20.0f, 0.0f, -50.0f,
         20.0f, 0.0f,  50.0f
    };

	static const GLfloat plane_texcoords[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };
	static const GLushort plane_indices[] =
    {
        0, 1, 2, 3
    };

    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices) + sizeof(plane_texcoords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plane_vertices), plane_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(plane_vertices), sizeof(plane_texcoords), plane_texcoords);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(plane_vertices)));
    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

    glGenBuffers(1, &cube_element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 7, GL_RGBA8, 64, 64);

    unsigned int * data = new unsigned int [64 * 64];
    unsigned int colors[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF00FFFF,
	                          0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF };
    for (int i = 0; i < 7; i++)  {
        for (int j = 0; j < 64 * 64; j++) {
            data[j] = colors[i];
        }
        glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, 64 >> i, 64 >> i, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 4.5f);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    delete [] data;

}

void reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
    aspect = float(width) / float(height);
}


void display(void)
{
	static const unsigned int start_time = GetTickCount();
    float t = float((GetTickCount() - start_time)) / float(0x3FFF);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDisable(GL_CULL_FACE);

	static glm::mat4 tc_matrix;
    tc_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -60.0f)) * 
		        glm::rotate(glm::mat4(1.0f), 80.0f * 3.0f * 0.03f, glm::vec3(1.0f, 0.0f, 0.0f));
    tc_matrix = glm::perspective(35.0f, 1.0f / aspect, 0.1f, 700.0f) * tc_matrix;
    glUniformMatrix4fv(skybox_rotate_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_element_buffer);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
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
	glutCreateWindow("Mipmap");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


