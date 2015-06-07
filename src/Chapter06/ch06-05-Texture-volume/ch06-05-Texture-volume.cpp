#pragma  comment (lib, "vermilion32_d.lib")

#include <common/vLoadTexture.h>
#include <common/shader.h>

#include <gl/glew.h>
#include <gl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

GLuint tex, tex_loc;
GLuint vao, vbo, ibo, program = 0;
Shader triangleShader("triangle shader");  
const int Width = 1200, Height = 1000; 
GLuint tc_rotate_loc;

static const GLfloat VertexData[] = 
{  
	 1.0f, -1.0f, 
	-1.0f, -1.0f,	
	-1.0f,  1.0f,	
	 1.0f,  1.0f,	

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
};	
static const GLsizei VertexSize = sizeof(VertexData);


void init_shader();
void init_buffer();
void init_vertexArray();
void init_texture();

void init(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //�ӿڱ任

}

void display(void)
{
	//Clear the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);

	//Bind the vao, which manage status we when to render
	glUseProgram(program);
	glBindVertexArray(vao);  

	float t = glutGet(GLUT_ELAPSED_TIME)/ 10000.0f;

	glm::mat4 tc_matrix = glm::rotate(glm::mat4(1.0), glm::radians(t * 170.0f), glm::vec3(1.0, 0.0, 0.0)) *
		                  glm::rotate(glm::mat4(1.0), glm::radians(t * 137.0f), glm::vec3(0.0, 1.0, 0.0))  *
		                  glm::rotate(glm::mat4(1.0), glm::radians(t * 93.0f),  glm::vec3(0.0, 0.0, 1.0));

	glUniformMatrix4fv(tc_rotate_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));
	
	glBindTexture(GL_TEXTURE_3D, tex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glutSwapBuffers();
	glutPostRedisplay();
}


int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Triangle");

	//glewInit() use first in order using other function
	GLenum res = glewInit(); 
	if (res != GLEW_OK) 
		std::cout << "Init the glew Error" << glewGetErrorString(res) << std::endl;

	init();
	glutDisplayFunc(display); 
	glutIdleFunc(display);
	glutReshapeFunc(reshape); 
	glutMainLoop();    
	return 0;
}


void init_shader()
{
	triangleShader.init();
	triangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	triangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	triangleShader.link();
	triangleShader.use();
	program = triangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
	tc_rotate_loc = glGetUniformLocation(program, "tc_rotate");
}


void init_buffer()
{
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void init_vertexArray()
{


	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT,0, (GLvoid*)(8 * sizeof(GLfloat) + NULL) );

}



void init_texture()
{
	vglImageData image;
	tex = vglLoadTexture("../../../media/textures/cloud.dds", 0, &image);
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	vglUnloadImage(&image);
}