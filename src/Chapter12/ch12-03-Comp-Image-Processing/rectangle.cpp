#include "Rectangle.h"
#include "ogl/loadTexture.h"


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

static const float verts[] =
{
	-0.75f, -0.75f, 0.5f, 1.0f,
	0.75f, -0.75f, 0.5f, 1.0f,
	0.75f, 0.75f, 0.5f, 1.0f,
	-0.75f, 0.75f, 0.5f, 1.0f,
};

void Rectangle::init_buffer()
{


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Rectangle::init_vertexArray()
{
	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);

	glGenBuffers(1, &render_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
}

}