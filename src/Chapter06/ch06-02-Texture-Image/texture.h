#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ogl/oglShader.h"
#include "ogl/loadTexture.h"

const int VertexCount = 6;

class Texture
{

public:
	Texture():vao(-1), vbo(-1), ibo(-1), program(-1), m_VertexCount(0), 
	           m_IndexCount(0) , TextureShader("Texture Shader") {}
	~Texture() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
       init_buffer();
	   init_vertexArray();
	   init_shader();
	   init_texture();
	}

	/////////////////////////////Render/////////////////////////////
	void Render()
	{
		glUseProgram(program);
		glBindVertexArray(vao);  

		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
	void init_texture();

private:

	GLuint tex, tex_loc;
	GLuint vao, vbo, ibo, program;
	OGLShader TextureShader;

	int m_VertexCount;
	int m_IndexCount;
};


static const GLfloat VertexData[] = 
{  
	0.75f, -0.75f, 
	-0.75f, -0.75f,	
	-0.75f,  0.75f,	
	0.75f,  0.75f,	

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
};	
static const GLsizei VertexSize = sizeof(VertexData);

void Texture::init_buffer()
{
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Texture::init_vertexArray()
{
	glGenVertexArrays(1, &vao);  
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(GLfloat) + NULL) );
}


void Texture::init_shader()
{
	TextureShader.init();
	TextureShader.attach(GL_VERTEX_SHADER, "Texture.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "Texture.frag");
	TextureShader.link();
	TextureShader.use();
	program = TextureShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
}


void Texture::init_texture()
{
   tex = loadTexture("../../../media/textures/test.dds");
}

#endif