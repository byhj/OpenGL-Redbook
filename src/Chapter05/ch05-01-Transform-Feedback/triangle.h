#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ogl/oglShader.h"

const int VertexCount = 6;

class Triangle
{

public:
	Triangle():vao(-1), vbo(-1), ibo(-1), program(-1), m_VertexCount(0), 
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
	    //Bind the vao, which manage status we when to render
		glUseProgram(program);
		glBindVertexArray(vao);  

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0, 0.0, -5.0));
		glm::mat4 view  = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 proj  = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
		glm::mat4 mvp   = proj * view;

		////////////////Update the model matrix when draw the every triangle///////////////
		
		// DrawArrays
		glUniformMatrix4fv(uniform.mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp) );
		glUniformMatrix4fv(uniform.model_loc, 1, GL_FALSE, glm::value_ptr(model) );
		glDrawArrays(GL_TRIANGLES, 0, 3); 

		// DrawElements
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 0.0, -5.0));
		glUniformMatrix4fv(uniform.model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);

		// DrawElementsBaseVertex
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0, 0.0, -5.0));
		glUniformMatrix4fv(uniform.model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL, 0);
		//the indices[i] + basevertex(the last element)

		//DrawArraysInstanced
		model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0, 0.0, -5.0));
		glUniformMatrix4fv(uniform.model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

		glUseProgram(0);
		glBindVertexArray(0);  
	}

	void Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();

private:

    struct Uniform
	{
		GLuint model_loc;
		GLuint mvp_loc;
	};
	Uniform uniform;

	GLuint vao, vbo, ibo, program;
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
static const GLsizeiptr VertexSize = sizeof(VertexData);

static const GLfloat ColorData[] = 
{  
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f                                                   //white
};
static const GLsizeiptr ColorSize = sizeof(ColorData);

static const GLushort IndexData[] =
{
	0, 1, 2 
};
static const GLsizeiptr IndexSize = sizeof(IndexData);

void Triangle::init_buffer()
{

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
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(VertexSize + NULL) );

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	uniform.mvp_loc   = glGetUniformLocation(program, "mvp");
	uniform.model_loc = glGetUniformLocation(program, "model");
}


#endif


