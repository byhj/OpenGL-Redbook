#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>

using namespace std;

#include <shader.h>
#include <vbm.cpp>
#include <loadTexture.cpp>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

const int Width = 800, Height = 640; //窗口宽高
float aspect;
GLuint skybox_prog, object_prog;
GLuint vao, base_prog;
GLuint cube_vbo, cube_element_buffer;
GLuint tex, gSampler;
GLint skybox_rotate_loc;
GLint mvp_loc,  mv_loc;
VBObject object;
Shader skyboxShader("skybox Shader");
Shader objectShader("skybox Shader");
void load_shaders()
{
	skyboxShader.init();
	skyboxShader.attach(GL_VERTEX_SHADER, "skybox.vert");
	skyboxShader.attach(GL_FRAGMENT_SHADER, "skybox.frag");
	skyboxShader.link();
	skyboxShader.use();
	base_prog = skyboxShader.program;
	objectShader.init();
	objectShader.attach(GL_VERTEX_SHADER, "object.vert");
	objectShader.attach(GL_FRAGMENT_SHADER, "object.frag");
	objectShader.link();
	objectShader.use();
	object_prog = objectShader.program;
    gSampler = glGetUniformLocation(base_prog, "gSampler");  //sampler是shader获取纹理数据的方法
    skybox_rotate_loc = glGetUniformLocation(skybox_prog, "tc_rotate");
    mvp_loc = glGetUniformLocation(object_prog, "mat_mvp");
    mv_loc = glGetUniformLocation(object_prog, "mat_mv");
	glUniform1i(gSampler, 0); //指定采样器索引为0；

}

void init(void)
{
	load_shaders();
    glClearColor(0.0f, 0.25f, 0.3f, 1.0f);
    glClearDepth(1.0f);
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    static const GLfloat cube_vertices[] =
    {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f
    };

    static const GLushort cube_indices[] =
    {
        0, 1, 2, 3, 6, 7, 4, 5,         // First strip
        2, 6, 0, 4, 1, 5, 3, 7          // Second strip
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cube_element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	vglImageData image;
	tex = vglLoadTexture("../media/textures/TantolundenCube.dds", 0, &image);
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    object.LoadFromVBM("../media/objects/torus.vbm", 0, 1, 2);
}

void display(void)
{
    static const unsigned int start_time = GetTickCount();
    float t = float((GetTickCount() - start_time)) / float(0x3FFF);
    static const glm::vec3 X(1.0f, 0.0f, 0.0f);
    static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
    static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glUseProgram(skybox_prog);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glm::mat4 tc_matrix = glm::mat4(1.0f);
     //tc_matrix = glm::rotate(80.0f * 3.0f * t, Y);// * glm::rotate(22.0f, Z);
    tc_matrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f) * tc_matrix;

    glUniformMatrix4fv(skybox_rotate_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_element_buffer);

    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glUseProgram(object_prog);
    tc_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)) 
	         	* glm::rotate(glm::mat4(1.0f), 80.0f * 3.0f * t, Y) 
				* glm::rotate(glm::mat4(1.0f), 70.0f * 3.0f * t, Z);

	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));
    tc_matrix = glm::perspective(35.0f, 1.0f / aspect, 0.1f, 100.0f) * tc_matrix;
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));
    glClear(GL_DEPTH_BUFFER_BIT);

    object.Render();
	glutSwapBuffers();
}

void finalize(void)
{
    glUseProgram(0);
    glDeleteProgram(skybox_prog);
    glDeleteProgram(object_prog);
    glDeleteTextures(1, &tex);
    glDeleteVertexArrays(1, &tex);
}

void reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
    aspect = float(height) / float(width);
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
	glutCreateWindow("Cube Map");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
