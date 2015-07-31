#include "Rectangle.h"
#include <glm/glm.hpp>
#include "ogl/loadTexture.h"

const int VertexCount = 6;

namespace byhj
{

void Rectangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Rectangle::Render()
{
	glUseProgram(program);
	glBindVertexArray(vao);

	glUniform1i(tex_loc, 0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
	glUseProgram(program);
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
	RectangleShader.attach(GL_VERTEX_SHADER,   "texture.vert");
	RectangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	RectangleShader.link();
	program = RectangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
}

void Rectangle::init_buffer()
{
	m_VertexCount = VertexCount;
	static const GLfloat VertexData[] =
	{
		0.75f, -0.75f,
		-0.75f, -0.75f,
		-0.75f, 0.75f,
		0.75f, 0.75f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	static const GLsizei VertexSize = sizeof(VertexData);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Rectangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(GLfloat)+NULL));
	
	glBindVertexArray(0);
}


void Rectangle::init_texture()
{
	tex = loadTexture("../../../media/textures/test.dds");
}

}