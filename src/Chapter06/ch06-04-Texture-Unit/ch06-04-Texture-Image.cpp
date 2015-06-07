#include <common/loadTexture.h>
#include <common/shader.h>

#include <gl/glew.h>
#include <gl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

GLuint tex0, tex1, tex0_loc, tex1_loc;
GLuint vao, vbo, ibo, program = 0;
Shader triangleShader("triangle shader");  
const int Width = 1200, Height = 800; 

static const GLfloat VertexData[] = 
{  
	//Position         Texcoord
	 0.75f, -0.75f,   0.0f, 0.0f,
	-0.75f, -0.75f,   1.0f, 0.0f,
	-0.75f,  0.75f,   1.0f, 1.0f,
	 0.75f,  0.75f,   0.0f, 1.0f,
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

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
	tex0_loc = glGetUniformLocation(program, "tex0");
	tex1_loc = glGetUniformLocation(program, "tex1");

	//set the GL_TEXTURE_2D index 
	glUniform1i(tex0_loc, 0);
	glUniform1i(tex1_loc, 1);
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat) + NULL) );

	glBindVertexArray(0);
}



void init_texture()
{
	tex0 = loadTexture("../../../media/textures/test.dds");
	tex1 = loadTexture("../../../media/textures/test3.dds");
}