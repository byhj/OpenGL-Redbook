#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "ogl/oglShader.h"

const int VertexCount = 6;

class Triangle
{

public:
	Triangle():vao(-1), vbo(-1), program(-1), m_VertexCount(0), 
	           m_IndexCount(0) , TriangleShader("Triangle Shader") {}
	~Triangle() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
       init_buffer();
	   init_vertexArray();
	   init_shader();
	}

	/////////////////////////////Render/////////////////////////////
	void Render()
	{
		glUseProgram(program);
		glBindVertexArray(vao);  
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);  
	}

	void Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	GLuint vao, vbo, program;
	OGLShader TriangleShader;
	int m_VertexCount;
	int m_IndexCount;
};

static const GLfloat VertexData[] = 
{  
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	1.0f,  1.0f, 0.0f, 1.0f,
};
static const GLsizei VertexSize = sizeof(VertexData);

static const GLfloat ColorData[] = 
{  
	1.0f, 0.0f, 0.0f, 1.0f, // red
	0.0f, 1.0f, 0.0f, 1.0f, //green
	0.0f, 0.0f, 1.0f, 1.0f  //blue                                                      
};
static const GLfloat ColorSize = sizeof(ColorData);


void Triangle::init_buffer()
{
	m_VertexCount = 3;

	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize + ColorSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, VertexSize, VertexData);
	glBufferSubData(GL_ARRAY_BUFFER, VertexSize, ColorSize, ColorData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(VertexSize + NULL) );

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif


