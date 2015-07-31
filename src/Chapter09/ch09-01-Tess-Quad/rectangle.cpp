#include "Rectangle.h"
#include <glm/glm.hpp>
#include <glfw/glfw3.h>

namespace byhj
{

void Rectangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Rectangle::Render()
{
	//Bind the vao, which manage status we when to render
	glUseProgram(program);
	glBindVertexArray(vao);

	float t = glfwGetTime();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(t * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f);
	glm::mat4 mvp =  proj * view * model;

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, &mvp[0][0]);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_PATCHES, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Rectangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}



void Rectangle::init_shader()
{
	RectangleShader.init();
	RectangleShader.attach(GL_VERTEX_SHADER, "quad.vert");
	RectangleShader.attach(GL_TESS_CONTROL_SHADER, "quad.tcs");
	RectangleShader.attach(GL_TESS_EVALUATION_SHADER, "quad.tes");
	RectangleShader.attach(GL_FRAGMENT_SHADER, "quad.frag");
	RectangleShader.link();
	program = RectangleShader.GetProgram();
}

static const GLfloat VertexData[] =
{
	-0.5, -0.5,
	0.5, -0.5,
	0.5, 0.5,
	-0.5, 0.5,
};
static const GLsizei VertexSize = sizeof(VertexData);

void Rectangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Rectangle::init_vertexArray()
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

}