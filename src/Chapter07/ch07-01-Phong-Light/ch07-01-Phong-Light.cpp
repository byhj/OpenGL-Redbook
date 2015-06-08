#include <common/shader.h>
#include <GL/freeglut.h>
#include <common/vmath.h>
#include <common/vbm.cpp>
#include <iostream>

void init_buffer();
void init_shader();

const int Width = 1000, Height = 800; 
VBObject g_object;
GLuint program;
Shader ObjectShader("Object Shader");

struct UniformLoc
{
	GLuint mv;
	GLuint proj;
	GLuint color_ambient;
	GLuint color_diffuse;
	GLuint color_specular;
};

UniformLoc uniformLoc;
void init()
{
	init_buffer();
	init_shader();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float time = glutGet(GLUT_ELAPSED_TIME) / 100000.0f;

	vmath::mat4 model = vmath::translate(0.0f, 0.0f, -4.0f) 
		                  * vmath::rotate(987.0f * time * 3.14159f, vmath::vec3(0.0f, 0.0f, 1.0f)) 
		                  * vmath::rotate(1234.0f * time * 3.14159f, vmath::vec3(1.0f, 0.0f, 0.0f));
	vmath::mat4 view = vmath::lookat(vmath::vec3(0.0f, 0.0f, 100.0f), 
		                             vmath::vec3(0.0f, 0.0f, 0.0f), 
									 vmath::vec3(0.0f, 1.0f, 0.0f) );

	vmath::mat4 mv = view * model;
	vmath::mat4 prj_matrix = vmath::perspective(60.0f, 1.333f, 0.1f, 1000.0f);

     glUseProgram(program);
	 glUniformMatrix4fv(uniformLoc.mv, 1, GL_FALSE, mv);
	 glUniformMatrix4fv(uniformLoc.proj, 1, GL_FALSE, prj_matrix);

	 glCullFace(GL_BACK);
	 glEnable(GL_CULL_FACE);
	 glEnable(GL_DEPTH_TEST);
	 glDepthFunc(GL_LEQUAL);

	 g_object.Render();

	 glutSwapBuffers();
	 glutPostRedisplay();
}

void shutdown()
{
	glUseProgram(0);
	glDeleteProgram(program);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("PointSprites");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		std::cout << "Init the glew Error" << glewGetErrorString(res) << std::endl;

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	shutdown();
	return 0;
}

void init_buffer()
{
	g_object.LoadFromVBM("../../../media/objects/torus.vbm", 0, 1, 2);
}

void init_shader()
{
	ObjectShader.init();
	ObjectShader.attach(GL_VERTEX_SHADER, "light.vert");
	ObjectShader.attach(GL_FRAGMENT_SHADER, "light.frag");
	ObjectShader.link();
	ObjectShader.interfaceInfo();

	program = ObjectShader.GetProgram();

	uniformLoc.mv = glGetUniformLocation(program, "model");
	uniformLoc.proj = glGetUniformLocation(program, "proj");
	uniformLoc.color_ambient = glGetUniformLocation(program, "color_ambient");
	uniformLoc.color_diffuse = glGetUniformLocation(program, "color_diffuse");
    uniformLoc.color_specular = glGetUniformLocation(program, "color_specular");
}