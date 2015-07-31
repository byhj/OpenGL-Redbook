#include "triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int VertexCount = 6;

namespace byhj
{

void Triangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Triangle::Render()
{
	//Bind the vao, which manage status we when to render
	glUseProgram(program);
	glBindVertexArray(vao);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0, 0.0, -5.0));
	glm::mat4 view  = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 proj  = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
	glm::mat4 projView   = proj * view;
	glm::mat4 mvp = glm::mat4(1.0f);

	////////////////Update the model matrix when draw the every triangle///////////////

	// DrawArrays
	mvp = projView * model;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// DrawElements
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 0.0, -5.0));
	mvp = projView * model;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);

	// DrawElementsBaseVertex
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -5.0));
	mvp = projView * model;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 0);
	//the indices[i] + base vertex(the last element)

	//DrawArraysInstanced
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0, 0.0, -5.0));
	mvp = projView * model;
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Triangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}



void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	mvp_loc = glGetUniformLocation(program, "mvp");
}

///////////////////////// Data /////////////////////////////////////
static const GLfloat VertexData[] =
{
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
};
static const GLsizeiptr VertexSize = sizeof(VertexData);

static const GLfloat ColorData[] =
{
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f                                                  
};
static const GLsizeiptr ColorSize = sizeof(ColorData);

static const GLushort IndexData[] =
{
	0, 1, 2
};
static const GLsizeiptr IndexSize = sizeof(IndexData);

//////////////////////////////////////////////////////////////////////

void Triangle::init_buffer()
{
	m_VertexCount = VertexCount;

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

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	//Set the shader data interface
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(VertexSize + NULL));

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


}


