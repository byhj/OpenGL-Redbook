#include "Rectangle.h"
#include <glm/glm.hpp>

const int VertexCount = 6;

namespace byhj
{

void Rectangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Rectangle::Render()
{
	// Activate the compute program and bind the output texture image
	glUseProgram(compute_prog);
	glBindImageTexture(0, output_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(8, 16, 1);

	////////////////////////////////////////////////////////
	// Now bind the texture for rendering _from_
	glBindTexture(GL_TEXTURE_2D, output_image);

	// Clear, select the rendering program and draw a full screen quad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(render_prog);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glUseProgram(0);
}

void Rectangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}



void Rectangle::init_shader()
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

	// This is the texture that the compute program will write into
	glGenTextures(1, &output_image);
	glBindTexture(GL_TEXTURE_2D, output_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rectangle::init_buffer()
{
	static const float verts[] =
	{
		-1.0f, -1.0f, 0.5f, 1.0f,
		1.0f, -1.0f, 0.5f, 1.0f,
		1.0f, 1.0f, 0.5f, 1.0f,
		-1.0f, 1.0f, 0.5f, 1.0f,
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Rectangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

}