#include <gl/glew.h>
#include <gl/freeglut.h>
#include <common/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GLuint vao, vbo, ibo, program = 0;
Shader triangleShader("triangle shader");  
const int Width = 1200, Height = 800; 
GLuint model_loc, mvp_loc;

static const GLfloat VertexData[] = 
{  
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

static const GLfloat ColorData[] = 
{  
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f                                                   //white
};
static const GLfloat ColorSize = sizeof(ColorData);

static const GLushort IndexData[] =
{
	0, 1, 2 
};
static const GLsizei IndexSize = sizeof(IndexData);

void init_shader();
void init_buffer();
void init_vertexArray();

void init(void)
{
	init_shader();
	init_buffer();
	init_vertexArray();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //设置颜色缓存为黑色
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //视口变换

}

void display(void)
{
	//Clear the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	//Bind the vao, which manage status we when to render
	glUseProgram(program);
	glBindVertexArray(vao);  

	glm::mat4 model= glm::translate(glm::mat4(1.0f), glm::vec3(-3.0, 0.0, -5.0));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 proj = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
	glm::mat4 mvp  = proj * view;

	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(model_loc,1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 3); 

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 0.0, -5.0));
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);

	// DrawElementsBaseVertex
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -5.0));
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 0);
	//the indices[i] + basevertex(the last element)

	//DrawArraysInstanced
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0, 0.0, -5.0));
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

	glutSwapBuffers();
	glutPostRedisplay();

	glUseProgram(0);
	glBindVertexArray(0);  
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
	program = triangleShader.GetProgram();
	mvp_loc = glGetUniformLocation(program, "mvp");
	model_loc = glGetUniformLocation(program, "model");
}


void init_buffer()
{
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize + ColorSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, VertexSize, VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize, ColorSize, ColorData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, IndexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void init_vertexArray()
{
	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(VertexSize + NULL) );

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


