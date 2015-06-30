#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "ogl/oglShader.h"

const int VertexCount = 6;

class Triangle
{

public:
	Triangle():vao(-1), vbo(-1), program(-1), m_VertexCount(0), m_IndexCount(0)
	          , TriangleShader("Triangle Shader") {}
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
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
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


void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
}

void Triangle::init_buffer()
{
	m_VertexCount = VertexCount;

	static const glm::vec2 VertexData[VertexCount] = 
	{  
		glm::vec2(-0.90f, -0.90f), // Triangle 1
		glm::vec2( 0.85f, -0.90f), 
		glm::vec2(-0.90f,  0.85f), 

		glm::vec2( 0.90f, -0.85f),  // Triangle 2
		glm::vec2( 0.90f,  0.90f), 
		glm::vec2(-0.85f,  0.90f), 
	};

	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#endif