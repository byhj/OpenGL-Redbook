#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl/oglShader.h"
#include "ogl/mat.h"
#include "Shapes/Teapot.h"

class Triangle
{

public:
	Triangle():vao(-1), vbo(-1), program(-1),
		        Inner(1.0f), Outer(1.0f), TriangleShader("Triangle Shader") {}
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
	void Render(float aspect)
	{
		//Bind the vao, which manage status we when to render
		glUseProgram(program);
		glBindVertexArray(vao);  

		mat4  projection = Perspective( 60.0, aspect, 5, 10 );
		//     mat4  projection = Frustum( -3, 3, -3, 3, 5, 10 );
		glUniformMatrix4fv(glGetUniformLocation( program, "P" ), 1, GL_TRUE, projection );

		float t = glfwGetTime();
		mat4  modelview = Translate( -0.2625f, -1.575f, -1.0f );
		modelview *= Translate( 0.0f, 0.0f, -7.5f );
		glUniformMatrix4fv( glGetUniformLocation( program, "MV" ), 1, GL_TRUE, modelview );

		glPatchParameteri(GL_PATCH_VERTICES, NumTeapotVerticesPerPatch);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements( GL_PATCHES, NumTeapotVertices, GL_UNSIGNED_INT, 0);

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

	GLuint vao, vbo, ibo, program;
	OGLShader TriangleShader;
	GLuint  Inner_loc;  // Inner tessellation paramter
	GLuint  Outer_loc;  // Outer tessellation paramte
	GLfloat  Inner;
	GLfloat  Outer;

};


void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "teapot.vert");
	TriangleShader.attach(GL_TESS_CONTROL_SHADER, "teapot.tcs");
	TriangleShader.attach(GL_TESS_EVALUATION_SHADER, "teapot.tes");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "teapot.frag");
	TriangleShader.link();
	TriangleShader.use();
	TriangleShader.interfaceInfo();
	program = TriangleShader.GetProgram();

	Inner_loc = glGetUniformLocation(program, "Inner");
	Outer_loc = glGetUniformLocation(program, "Outer");
	glUniform1f( Inner_loc, Inner);
	glUniform1f( Outer_loc, Outer);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(TeapotVertices),  TeapotVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TeapotIndices), TeapotIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

#endif
