#include "cube.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#define USE_PRIMITIVE_RESTART//¿ªÆôÍ¼ÔªÖÐ¶Ï

namespace byhj
{

void Cube::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Cube::Render()
{
	glUseProgram(program);
	glBindVertexArray(vao);


	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -5.0));
	model *= glm::rotate(glm::mat4(1.0f), 30.0f , glm::vec3(0.0f, 1.0f, 0.0f));
	//set the view and projection matrix

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
	glm::mat4 mvp  = projection * view;

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#ifdef USE_PRIMITIVE_RESTART   
	// When primitive restart is on, we can call one draw command
	glEnable(GL_PRIMITIVE_RESTART); 
	glPrimitiveRestartIndex(0xFFFF); 
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL); 
#else
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid *)(9 * sizeof(GLushort)));
#endif

	glBindVertexArray(0);
	glUseProgram(0);

}

void Cube::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}



void Cube::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "cube.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	modelLoc= glGetUniformLocation(program, "model_matrix");
	mvpLoc = glGetUniformLocation(program, "mvp_matrix");
}



static const GLfloat positions[] = 
{
	-1.0f, -1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};


static const GLfloat colors[] =
{
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f
};

// Indices for the triangle strips
static const GLushort indices[] =
{
	0, 1, 2, 3, 6, 7, 4, 5,         // First strip
	0xFFFF,                         // restart
	2, 6, 0, 4, 1, 5, 3, 7          // Second strip
};

void Cube::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions)+sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Cube::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}


}