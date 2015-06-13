#include <common/rebApp.h>
#include <common/shader.h>
#include <common/vmath.h>
#include <common/glDebug.h>

#include "vbm.h"

#define MAX_FRAMEBUFFER_WIDTH 2048
#define MAX_FRAMEBUFFER_HEIGHT 2048
#define FRUSTUM_DEPTH 2000.0f

class ImageApp: public byhj::Application
{
public:
	ImageApp(){};
	~ImageApp() {};

	void v_Init()
	{
		init_buffer();
		init_shader();
	}
	void v_Render()
	{
		float t;

		unsigned int current_time = GetTickCount();

		t = (float)(current_time & 0xFFFFF) / (float)0x3FFF;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind palette buffer
		glBindImageTexture(0, image_palette_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

		// Clear output image
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, output_texture_clear_buffer);
		glBindTexture(GL_TEXTURE_2D, output_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GetScreenWidth(), GetScreenHeight(), GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Bind output image for read-write
		glBindImageTexture(1, output_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

		// Render
		glUseProgram(render_scene_prog);

		vmath::mat4 model_matrix = vmath::translate(0.0f, 0.0f, -15.0f) *
			vmath::rotate(t * 360.0f, 0.0f, 0.0f, 1.0f) *
			vmath::rotate(t * 435.0f, 0.0f, 1.0f, 0.0f) *
			vmath::rotate(t * 275.0f, 1.0f, 0.0f, 0.0f);
		vmath::mat4 view_matrix = vmath::mat4::identity();
		vmath::mat4 projection_matrix = vmath::frustum(-1.0f, 1.0f, GetAspect(), GetAspect(), 1.0f, 40.f);

		glUniformMatrix4fv(render_scene_uniforms.model_matrix, 1, GL_FALSE, model_matrix);
		glUniformMatrix4fv(render_scene_uniforms.view_matrix, 1, GL_FALSE, view_matrix);
		glUniformMatrix4fv(render_scene_uniforms.projection_matrix, 1, GL_FALSE, projection_matrix);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		object.Render(0, 4 * 4 * 4);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glBindImageTexture(0, output_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

		glBindVertexArray(quad_vao);
		glUseProgram(resolve_program);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glutSwapBuffers();
		glutPostRedisplay();
	}
	void v_Shutdown()
	{
		glUseProgram(0);
	}
private:
	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

	VBObject object;
	Shader AppShader;
	Shader BlitShader;

	// Program to construct the linked list (renders the transparent objects)
	GLuint  list_build_program;

	// Color palette buffer texture
	GLuint  image_palette_buffer;
	GLuint  image_palette_texture;

	// Output image and PBO for clearing it
	GLuint  output_texture;
	GLuint  output_texture_clear_buffer;

	// Program to render the scene
	GLuint render_scene_prog;
	struct
	{
		GLint aspect;
		GLint time;
		GLint model_matrix;
		GLint view_matrix;
		GLint projection_matrix;
	} render_scene_uniforms;

	// Program to resolve 
	GLuint resolve_program;

	// Full Screen Quad
	GLuint  quad_vbo;
	GLuint  quad_vao;
};

CALL_MAIN(ImageApp);


void ImageApp::init_buffer()
{
	// Create palette texture
	glGenBuffers(1, &image_palette_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER, image_palette_buffer);
	glBufferData(GL_TEXTURE_BUFFER, 256 * 4 * sizeof(float), NULL, GL_STATIC_DRAW);
	glGenTextures(1, &image_palette_texture);
	glBindTexture(GL_TEXTURE_BUFFER, image_palette_texture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, image_palette_buffer);

	vmath::vec4 * data = (vmath::vec4 *)glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < 256; i++)
	{
		data[i] = vmath::vec4((float)i);
	}
	glUnmapBuffer(GL_TEXTURE_BUFFER);

	// Create head pointer texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &output_texture);
	glBindTexture(GL_TEXTURE_2D, output_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindImageTexture(0, output_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);

	// Create buffer for clearing the head pointer texture
	glGenBuffers(1, &output_texture_clear_buffer);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, output_texture_clear_buffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint), NULL, GL_STATIC_DRAW);

	data = (vmath::vec4 *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	memset(data, 0x00, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint));
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	// Create VAO containing quad for the final blit
	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);

	static const GLfloat quad_verts[] =
	{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
	};

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), quad_verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glClearDepth(1.0f);

	object.LoadFromVBM("../../../media/objects/unit_pipe.vbm", 0, 1, 2);
}

void ImageApp::init_vertexArray()
{

}

void ImageApp::init_shader()
{
	AppShader.init();
	AppShader.attach(GL_VERTEX_SHADER, "base.vert");
	AppShader.attach(GL_FRAGMENT_SHADER, "base.frag");
	AppShader.link();
	render_scene_prog  = AppShader.GetProgram();
	render_scene_uniforms.model_matrix = glGetUniformLocation(render_scene_prog, "model_matrix");
	render_scene_uniforms.view_matrix = glGetUniformLocation(render_scene_prog, "view_matrix");
	render_scene_uniforms.projection_matrix = glGetUniformLocation(render_scene_prog, "projection_matrix");
	render_scene_uniforms.aspect = glGetUniformLocation(render_scene_prog, "aspect");
	render_scene_uniforms.time = glGetUniformLocation(render_scene_prog, "time");

	BlitShader.init();
	BlitShader.attach(GL_VERTEX_SHADER, "blit.vert");
	BlitShader.attach(GL_FRAGMENT_SHADER, "blit.frag");
	BlitShader.link();
	resolve_program = BlitShader.GetProgram();

}

void ImageApp::init_texture()
{


}