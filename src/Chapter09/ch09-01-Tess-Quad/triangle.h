#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl/oglShader.h"

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
		//Bind the vao, which manage status we when to render
		glUseProgram(program);
		glBindVertexArray(vao);  

		float t = glfwGetTime();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.0f ) ) 
			            * glm::rotate(glm::mat4(1.0f), glm::radians(t * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(45.0f, 1.0f, 1.0f, 100.0f);
		glm::mat4 mvp =  proj * view * model;

		glUniformMatrix4fv( glGetUniformLocation( program, "mvp" ), 1, GL_FALSE, &mvp[0][0]);
		glPatchParameteri(GL_PATCH_VERTICES, 4);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_PATCHES, 0, 4);
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
	TriangleShader.attach(GL_VERTEX_SHADER, "quad.vert");
	TriangleShader.attach(GL_TESS_CONTROL_SHADER, "quad.tcs");
	TriangleShader.attach(GL_TESS_EVALUATION_SHADER, "quad.tes");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "quad.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
}
static const GLfloat VertexData[] = 
{  
	-0.5, -0.5,
	 0.5, -0.5,
	 0.5,  0.5,
	-0.5,  0.5,
};	
static const GLsizei VertexSize = sizeof(VertexData);

void Triangle::init_buffer()
{
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize , VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
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

#endif





