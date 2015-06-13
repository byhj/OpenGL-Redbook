#include <common/rebApp.h>
#include <common/shader.h>
#include <common/vmath.h>
#include <common/vbm.cpp>
#include <common/vLoadTexture.h>

class ImageApp: public byhj::Application
{
public:
	ImageApp(){};
	~ImageApp() {};

	void v_Init()
	{
		init_buffer();
		init_vertexArray();
		init_shader();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
	}
	void v_Render()
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

		// Now bind the texture for rendering _from_
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, output_image);

		// Clear, select the rendering program and draw a full screen quad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(render_prog);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glutSwapBuffers();

	}
	void v_Shutdown()
	{
		glUseProgram(0);
	}
private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint  render_vao;
	GLuint  render_vbo;

	// Texture to process
	GLuint  input_image;
	// Texture for compute shader to write into
	GLuint  intermediate_image;
	GLuint  output_image;

	GLuint  compute_prog;
	GLuint  render_prog;

	Shader AppShader;
	Shader CompShader;

};

CALL_MAIN(ImageApp);


static const float verts[] =
{
	-1.0f, -1.0f, 0.5f, 1.0f,
	 1.0f, -1.0f, 0.5f, 1.0f,
	 1.0f,  1.0f, 0.5f, 1.0f,
	-1.0f,  1.0f, 0.5f, 1.0f,
};

void ImageApp::init_buffer()
{

}

void ImageApp::init_vertexArray()
{
	// This is the VAO containing the data to draw the quad (including its associated VBO)
	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &render_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render_vbo);
	static const float verts[] =
	{
		-1.0f, -1.0f, 0.5f, 1.0f,
		1.0f, -1.0f, 0.5f, 1.0f,
		1.0f,  1.0f, 0.5f, 1.0f,
		-1.0f,  1.0f, 0.5f, 1.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

}

void ImageApp::init_shader()
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
	input_image = vglLoadTexture("../../../media/textures/test1.dds", 0, NULL);

	glGenTextures(1, &intermediate_image);
	glBindTexture(GL_TEXTURE_2D, intermediate_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 1024, 1024);
	glBindTexture(GL_TEXTURE_2D, 0);

	// This is the texture that the compute program will write into
	glGenTextures(1, &output_image);
	glBindTexture(GL_TEXTURE_2D, output_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 1024, 1024);
	glBindTexture(GL_TEXTURE_2D, 0);
}

