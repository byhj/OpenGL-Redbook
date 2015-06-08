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
	-0.5, -0.5,
	 0.5, -0.5,
	 0.5,  0.5,
	-0.5,  0.5,
};	
static const GLsizei VertexSize = sizeof(VertexData);


void init_shader();
void init_buffer();
void init_vertexArray();

void init(void)
{
	init_shader();
	init_buffer();
	init_vertexArray();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //设置颜色缓存为黑色
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //视口变换

}

void display(void)
{
	//Clear the framebuffer
	glClear(GL_COLOR_BUFFER_BIT); 

	//Bind the vao, which manage status we when to render
	glUseProgram(program);
	glBindVertexArray(vao);  
	float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -2.0f ) ) 
		            * glm::rotate(glm::mat4(1.0f), glm::radians(t * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f);
	glm::mat4 mvp =  proj * view * model;

	glUniformMatrix4fv( glGetUniformLocation( program, "mvp" ), 1, GL_FALSE, &mvp[0][0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 4);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4)
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
	glutReshapeFunc(reshape); 
	glutMainLoop();    
	return 0;
}


void init_shader()
{
	triangleShader.init();
	triangleShader.attach(GL_VERTEX_SHADER, "quad.vert");
	triangleShader.attach(GL_TESS_CONTROL_SHADER, "quad.tcs");
	triangleShader.attach(GL_TESS_EVALUATION_SHADER, "quad.tes");
	triangleShader.attach(GL_FRAGMENT_SHADER, "quad.frag");
	triangleShader.link();
	program = triangleShader.GetProgram();
}


void init_buffer()
{
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize , VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void init_vertexArray()
{
	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


