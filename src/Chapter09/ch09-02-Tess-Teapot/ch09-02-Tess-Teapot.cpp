#include <gl/glew.h>
#include <gl/freeglut.h>

#include <common/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Shapes/Teapot.h>

GLuint vao, vbo, ibo, program = 0;
Shader triangleShader("triangle shader");  
const int Width = 1200, Height = 800; 
GLuint  mvp_loc;

GLuint  Inner_loc;  // Inner tessellation paramter
GLuint  Outer_loc;  // Outer tessellation paramte
GLfloat  Inner = 1.0;
GLfloat  Outer = 1.0;

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
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.2625f, -1.575f, -8.5f ) );
		
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(60.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 mvp =  proj * view * model;

	glUniformMatrix4fv( glGetUniformLocation( program, "mvp" ), 1, GL_FALSE, &mvp[0][0]);
	glPatchParameteri(GL_PATCH_VERTICES, NumTeapotVerticesPerPatch);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements( GL_PATCHES, NumTeapotVertices, GL_UNSIGNED_INT, 0);

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
	triangleShader.attach(GL_VERTEX_SHADER, "teapot.vert");
	triangleShader.attach(GL_TESS_CONTROL_SHADER, "teapot.tcs");
	triangleShader.attach(GL_TESS_EVALUATION_SHADER, "teapot.tes");
	triangleShader.attach(GL_FRAGMENT_SHADER, "teapot.frag");
	triangleShader.link();
	triangleShader.interfaceInfo();
	program = triangleShader.GetProgram();
	Inner_loc = glGetUniformLocation(program, "Inner");
	Outer_loc = glGetUniformLocation(program, "Outer");
	glUniform1f( Inner_loc, Inner);
	glUniform1f( Outer_loc, Outer);
}


void init_buffer()
{
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TeapotVertices),  TeapotVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TeapotIndices), TeapotIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void init_vertexArray()
{
	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}


