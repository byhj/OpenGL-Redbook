#include <common/rebApp.h>
#include <common/shader.h>
#include <common/vmath.h>
#include <common/vbm.cpp>


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
		glBindImageTexture(0, output_image, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(8, 16, 1);

		// Now bind the texture for rendering _from_
		glBindTexture(GL_TEXTURE_2D, output_image);

		// Clear, select the rendering program and draw a full screen quad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(render_prog);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glutSwapBuffers();

	}
	void v_Shutdown()
	{
		glUseProgram(0);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();

	GLuint vao, vbo;
    GLuint  output_image;
	// Member variables
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
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ImageApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

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

	// This is the texture that the compute program will write into
	glGenTextures(1, &output_image);
	glBindTexture(GL_TEXTURE_2D, output_image);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, 256, 256);
}

