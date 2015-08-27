#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ogl/oglShader.h"
#include "ogl/loadTexture.h"
#include "ogl/vmath.h"

#define POINT_COUNT 200

GLuint sprite_texture;
GLint model_loc;
GLint proj_loc;

static inline float random_float()
{
	float res;
	unsigned int tmp;
	static unsigned int seed = 0x13371337;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

class Texture
{

public:
	Texture():vao(-1), vbo(-1), ibo(-1), program(-1), TextureShader("Texture Shader") {}
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
		glUseProgram(program);
		glBindVertexArray(vao);  

		float t = glfwGetTime() / 10.0f;

		static float q = 0.0f;
		static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
		static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
		static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

		// Setup
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);

		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		vmath::mat4 proj(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 8.0f));
		vmath::mat4 model = vmath::translate(0.0f, 0.0f, -2.0f) 
			              * vmath::rotate(t * 360.0f, Y) * rotate(t * 720.0f, Z);
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj);

		glBindTexture(GL_TEXTURE_2D, sprite_texture);

		//Enable it to set shader gl_PointSize work
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_POINT_SPRITE);
		glPointSize(32.0f);

		glDrawArrays(GL_POINTS, 0, POINT_COUNT); //ªÊ÷∆µ„
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
};


void Texture::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, POINT_COUNT * sizeof(vmath::vec4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Texture::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vmath::vec4 * vertex_positions = (vmath::vec4 *)glMapBuffer(
		                             GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	for (int n = 0; n < POINT_COUNT; n++)
		vertex_positions[n] = vmath::vec4(random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, 
		                                  random_float() * 2.0f - 1.0f, 1.0f);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}


void Texture::init_shader()
{
	TextureShader.init();
	TextureShader.attach(GL_VERTEX_SHADER, "pointsprites.vert");
	TextureShader.attach(GL_FRAGMENT_SHADER, "pointsprites.frag");
	TextureShader.link();
	TextureShader.use();
	program = TextureShader.GetProgram();

	tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex_loc, 0);
	model_loc = glGetUniformLocation(program, "model");
	proj_loc = glGetUniformLocation(program, "proj");
}


void Texture::init_texture()
{
	sprite_texture = loadTexture("../../../media/textures/sprite.tga");
	glGenerateMipmap(GL_TEXTURE_2D);
}

#endif



