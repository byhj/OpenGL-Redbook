#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ogl/oglShader.h"
#include "ogl/loadTexture.h"
#include "ogl/oglRandom.h"

#include "ogl/vLoadTexture.h"
#pragma  comment (lib, "vermilion32_d.lib")


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

		static float totalTime = 0.0f;
		static float lastTime = glfwGetTime();
		float currentTime = glfwGetTime();
		totalTime += currentTime - lastTime;

		static glm::vec4 ranColor = glm::vec4(1.0f);
		glm::mat4 tc_matrix = glm::rotate(glm::mat4(1.0), glm::radians(currentTime * 17.0f), glm::vec3(1.0, 0.0, 0.0)) *
			                  glm::rotate(glm::mat4(1.0), glm::radians(currentTime * 13.0f), glm::vec3(0.0, 1.0, 0.0))  *
			                  glm::rotate(glm::mat4(1.0), glm::radians(currentTime * 9.0f),  glm::vec3(0.0, 0.0, 1.0));

		glUniformMatrix4fv(tc_rotate_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));
	
		//We change the cloud color every second
		if (totalTime > 1.0f)
		{
			ranColor = random.GetColor();
			totalTime = 0.0f;
		}
		glUniform4fv(randomColor_loc, 1, &ranColor[0]);

		glBindTexture(GL_TEXTURE_3D, tex);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		lastTime = currentTime;
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
	GLuint tc_rotate_loc, randomColor_loc;
	GLuint tex, tex_loc;
	GLuint vao, vbo, ibo, program;
	OGLShader TextureShader;
	OGLRandom random;
	int m_VertexCount;
	int m_IndexCount;
};


static const GLfloat VertexData[] = 
{  
	1.0f, -1.0f, 
	-1.0f, -1.0f,	
	-1.0f,  1.0f,	
	1.0f,  1.0f,	

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

	tc_rotate_loc = glGetUniformLocation(program, "tc_rotate");
	randomColor_loc = glGetUniformLocation(program, "randomColor");
}


void Texture::init_texture()
{
	vglImageData image;
	tex = vglLoadTexture("../../../media/textures/cloud.dds", 0, &image);
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	vglUnloadImage(&image);
}

#endif
