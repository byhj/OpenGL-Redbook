#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "ogl/oglShader.h"
#include "ogl/loadTexture.h"

const int VertexCount = 6;

class Triangle
{

public:
	Triangle() {}
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
		// Activate the compute program and bind the output texture image
		glUseProgram(compute_prog);
		glBindImageTexture(0, input_image, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(1, intermediate_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(1, 1024, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		glBindImageTexture(0, intermediate_image, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		glBindImageTexture(1, output_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(1, 1024, 1);

		// Clear, select the rendering program and draw a full screen quad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Now bind the texture for rendering _from_
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, output_image);
		glUseProgram(render_prog);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	void Shutdown()
	{

	}

private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	GLuint  render_vao;
	GLuint  render_vbo;

	// Texture to process
	GLuint  input_image;
	// Texture for compute shader to write into
	GLuint  intermediate_image;
	GLuint  output_image;

	GLuint  compute_prog;
	GLuint  render_prog;

	OGLShader AppShader;
	OGLShader CompShader;
};



static const float verts[] =
{
	-0.75f, -0.75f, 0.5f, 1.0f,
	0.75f,  -0.75f, 0.5f, 1.0f,
	0.75f,   0.75f, 0.5f, 1.0f,
	-0.75f,  0.75f, 0.5f, 1.0f,
};

void Triangle::init_buffer()
{

}

void Triangle::init_shader()
{
	AppShader.init();
	AppShader.attach(GL_VERTEX_SHADER, "simple.vert");
	AppShader.attach(GL_FRAGMENT_SHADER, "simple.frag");
	AppShader.link();
	render_prog = AppShader.GetProgram();

	CompShader.init();
	CompShader.attach(GL_COMPUTE_SHADER, "simple.comp");
	CompShader.link();
	compute_prog = CompShader.GetProgram();

	// Load a texture to process
	input_image = loadTexture("../../../media/textures/test.dds");

	glGenTextures(1, &intermediate_image);
	glBindTexture(GL_TEXTURE_2D, intermediate_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 1024, 1024);

	// This is the texture that the compute program will write into
	glGenTextures(1, &output_image);
	glBindTexture(GL_TEXTURE_2D, output_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 1024, 1024);
}

void Triangle::init_vertexArray()
{
	// This is the VAO containing the data to draw the quad (including its associated VBO)
	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);

	glGenBuffers(1, &render_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

}

#endif

