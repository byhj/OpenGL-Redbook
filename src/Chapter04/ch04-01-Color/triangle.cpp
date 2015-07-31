#include "triangle.h"
#include <glm/glm.hpp>

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
	glUseProgram(program);
	glBindVertexArray(vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
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
}

static const GLfloat VertexData[] =
{
	-0.90, -0.90,
	0.85, -0.90,
	-0.90, 0.85,

	0.90, -0.85,
	0.90, 0.90,
	-0.85, 0.90,
};

static const GLsizei VertexSize = sizeof(VertexData);

static const GLubyte ColorData[] =
{
	255, 0, 0, 255,
	0, 255, 0, 255,
	0, 0, 255, 255,
	10, 10, 10, 255,
	100, 100, 100, 255,
	255, 255, 255, 255,
};
static const GLfloat ColorSize = sizeof(ColorData);

void Triangle::init_buffer()
{
	m_VertexCount = VertexCount;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, VertexSize + ColorSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, VertexSize, VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize, ColorSize, ColorData);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//Notice BYTE map to float [0.0, 1.0], so 255 is 1.0, 0 is 0.0
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)(VertexSize + NULL));

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


}