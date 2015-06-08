#include<gl/glew.h>
#include <common/shader.h>
#include <common/vbm.cpp>
#include <common/vmath.h>
#include <common/vLoadTexture.h>

#include<gl/freeglut.h>
#include <iostream>


const int Width = 1000, Height = 800; //窗口宽高
float aspect;
GLuint skybox_prog, object_prog;
GLuint vao, base_prog;
GLuint cube_vbo, cube_ibo;
GLuint tex, tex_loc;
GLint skybox_rotate_loc,  proj_loc,  mv_loc;
VBObject object;
Shader skyboxShader("skybox Shader");
Shader objectShader("object Shader");

void init_buffer();
void init_shader();
void init_texture();
void init_vertexArray();

void init(void)
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	glClearColor(0.0f, 0.25f, 0.3f, 1.0f);
	glClearDepth(1.0f);
}
void display(void)
{
	static const unsigned int start_time = GetTickCount();
	float t = float((GetTickCount() - start_time)) / float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	vmath::mat4 tc_matrix(vmath::mat4::identity());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	//First we render cube box with texture_cube to set skybox
	glUseProgram(skybox_prog);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ibo);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// tc_matrix = vmath::rotate(80.0f * 3.0f * t, Y);// * vmath::rotate(22.0f, Z);
	tc_matrix = vmath::perspective(35.0f, 1.0f / aspect, 0.1f, 100.0f) * tc_matrix;

	glUniformMatrix4fv(skybox_rotate_loc, 1, GL_FALSE, tc_matrix);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ibo);

	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort) ) );

	///////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(object_prog);
	object.BindVertexArray();
	vmath::mat4 world = vmath::translate(vmath::vec3(0.0f, 0.0f, 30.0f)) 
		              * vmath::rotate(80.0f * 3.0f * t, Y)
			          * vmath::rotate(70.0f * 3.0f * t, Z);
	vmath::mat4 view = vmath::lookat(vmath::vec3(0.0f, 0.0f, 100.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f) );
	vmath::mat4 mv = view * world;

	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, mv);

	vmath::mat4 proj = vmath::perspective(35.0f, 1.0f / aspect, 0.1f, 100.0f);
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj);

	glClear(GL_DEPTH_BUFFER_BIT);

	object.Render();

	glutSwapBuffers();
	glutPostRedisplay();
}

void finalize(void)
{
	glDeleteProgram(skybox_prog);
	glDeleteProgram(object_prog);
	glDeleteTextures(1, &tex);
	glDeleteVertexArrays(1, &vao);
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Cube Map");
	GLenum res = glewInit();  
	if (res != GLEW_OK) 
		std::cout << "Init the glew Error" << glewGetErrorString(res) << std::endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

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

void init_buffer()
{
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cube_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	object.LoadFromVBM("../../../media/objects/torus.vbm", 0, 1, 2);
}

void init_shader()
{
	skyboxShader.init();
	skyboxShader.attach(GL_VERTEX_SHADER, "skybox.vert");
	skyboxShader.attach(GL_FRAGMENT_SHADER, "skybox.frag");
	skyboxShader.link();
	skyboxShader.use();
	skybox_prog = skyboxShader.GetProgram();
	skybox_rotate_loc = glGetUniformLocation(skybox_prog, "tc_rotate");
	
	objectShader.init();
	objectShader.attach(GL_VERTEX_SHADER, "object.vert");
	objectShader.attach(GL_FRAGMENT_SHADER, "object.frag");
	objectShader.link();
	objectShader.use();
	objectShader.interfaceInfo();
	object_prog = objectShader.GetProgram();
	//tex_loc = glGetUniformLocation(base_prog, "tex");  //sampler是shader获取纹理数据的方法
	proj_loc = glGetUniformLocation(object_prog, "proj");
	mv_loc = glGetUniformLocation(object_prog, "mv");
	//glUniform1i(tex_loc, 0); 

}
void init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void init_texture()
{
	vglImageData image;
	tex = vglLoadTexture("../../../media/textures/TantolundenCube.dds", 0, &image);
	GLenum e;
	e = glGetError();

	vglUnloadImage(&image);
}

