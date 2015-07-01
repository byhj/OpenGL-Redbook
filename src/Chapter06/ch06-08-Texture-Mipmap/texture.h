#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ogl/oglShader.h"
#include "ogl/loadTexture.h"
#include "ogl/vmath.h"

const int VertexCount = 6;

class Texture
{

public:
	Texture():vao(-1), vbo(-1), ibo(-1), program(-1),TextureShader("Texture Shader") {}
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
	void Render(float aspect)
	{
		static const unsigned int start_time = GetTickCount();
		float t = float((GetTickCount() - start_time)) / float(0x3FFF);
		static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
		static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
		static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		glDisable(GL_CULL_FACE);

		vmath::mat4 tc_matrix(vmath::mat4::identity());
		tc_matrix = vmath::translate(vmath::vec3(0.0f, 0.0f, -60.0f)) * vmath::rotate(80.0f * 3.0f * 0.03f, X);
		tc_matrix = vmath::perspective(35.0f, 1.0f / aspect, 0.1f, 700.0f) * tc_matrix;
		glUniformMatrix4fv(rotate_loc, 1, GL_FALSE, tc_matrix);

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
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
	GLuint rotate_loc;
	GLuint texture;
};

static const GLfloat plane_vertices[] =
{
	-20.0f, 0.0f, -50.0f,
	-20.0f, 0.0f,  50.0f,
	20.0f, 0.0f, -50.0f,
	20.0f, 0.0f,  50.0f
};

static const GLfloat plane_texcoords[] =
{
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};
static const GLushort plane_indices[] =
{
	0, 1, 2, 3
};

void Texture::init_buffer()
{

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices) + sizeof(plane_texcoords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plane_vertices), plane_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(plane_vertices), sizeof(plane_texcoords), plane_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Texture::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(plane_vertices)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
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
	rotate_loc = glGetUniformLocation(program, "tc_rotate");
}


void Texture::init_texture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 7, GL_RGBA8, 64, 64);

	unsigned int * data = new unsigned int [64 * 64];
	unsigned int colors[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF00FFFF,
		                      0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF };
	for (int i = 0; i < 7; i++)  
	{
		for (int j = 0; j < 64 * 64; j++) 
			data[j] = colors[i];

		glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, 64 >> i, 64 >> i, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 4.5f);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	delete [] data;
}

#endif


