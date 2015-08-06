#include <common/rebApp.h>
#include <common/shader.h>
#include <common/vmath.h>

#include "vbm.h"

#define MAX_FRAMEBUFFER_WIDTH 2048
#define MAX_FRAMEBUFFER_HEIGHT 2048
#define FRUSTUM_DEPTH 2000.0f

using namespace vmath;

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
	}
	void v_reshape(int width, int height)
	{
		current_width = width;
		current_height = height;

		aspect = float(height) / float(width);
		glViewport(0, 0, current_width, current_height);
	}
	void v_Render()
	{

		float t;

		unsigned int current_time = GetTickCount();

		t = (float)(current_time & 0xFFFFF) / (float)0x3FFF;

		GLuint * data;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Reset atomic counter
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
		data = (GLuint *)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
		data[0] = 0;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

		// Clear head-pointer image
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
		glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, current_width, current_height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Bind head-pointer image for read-write
		glBindImageTexture(0, head_pointer_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

		// Bind linked-list buffer for write
		glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

		glUseProgram(render_scene_prog);

		vmath::mat4 model_matrix = vmath::translate(0.0f, 0.0f, -20.0f) *
			vmath::rotate(t * 360.0f, 0.0f, 0.0f, 1.0f) *
			vmath::rotate(t * 435.0f, 0.0f, 1.0f, 0.0f) *
			vmath::rotate(t * 275.0f, 1.0f, 0.0f, 0.0f);
		vmath::mat4 view_matrix = vmath::mat4::identity();
		vmath::mat4 projection_matrix = vmath::frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 40.f);

		glUniformMatrix4fv(render_scene_uniforms.model_matrix, 1, GL_FALSE, model_matrix);
		glUniformMatrix4fv(render_scene_uniforms.view_matrix, 1, GL_FALSE, view_matrix);
		glUniformMatrix4fv(render_scene_uniforms.projection_matrix, 1, GL_FALSE, projection_matrix);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		object.Render(0, 8 * 8 * 8);

		glDisable(GL_BLEND);

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

	Shader AppShader;
	Shader BlitShader;

	// Program to construct the linked list (renders the transparent objects)
	GLuint  list_build_program;

	// Head pointer image and PBO for clearing it
	GLuint  head_pointer_texture;
	GLuint  head_pointer_clear_buffer;
	// Atomic counter buffer
	GLuint  atomic_counter_buffer;
	// Linked list buffer
	GLuint  linked_list_buffer;
	GLuint  linked_list_texture;

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

	GLint current_width;
	GLint current_height;

	VBObject object;
	// Member variables
	float aspect;
};

CALL_MAIN(ImageApp);


void ImageApp::init_buffer()
{
	GLuint * data;
	render_scene_prog = -1;

	// Create head pointer texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &head_pointer_texture);
	glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindImageTexture(0, head_pointer_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

	// Create buffer for clearing the head pointer texture
	glGenBuffers(1, &head_pointer_clear_buffer);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	data = (GLuint *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	memset(data, 0x00, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint));
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	// Create the atomic counter buffer
	glGenBuffers(1, &atomic_counter_buffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

	// Create the linked list storage buffer
	glGenBuffers(1, &linked_list_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER, linked_list_buffer);
	glBufferData(GL_TEXTURE_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * 3 * sizeof(vec4), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	// Bind it to a texture (for use as a TBO)
	glGenTextures(1, &linked_list_texture);
	glBindTexture(GL_TEXTURE_BUFFER, linked_list_texture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, linked_list_buffer);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);

	static const GLfloat quad_verts[] =
	{
		-0.0f, -1.0f,
		1.0f, -1.0f,
		-0.0f,  1.0f,
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
	AppShader.attach(GL_VERTEX_SHADER, "build.vert");
	AppShader.attach(GL_FRAGMENT_SHADER, "build.frag");
	AppShader.link();
	render_scene_prog  = AppShader.GetProgram();
	render_scene_uniforms.model_matrix = glGetUniformLocation(render_scene_prog, "model_matrix");
	render_scene_uniforms.view_matrix = glGetUniformLocation(render_scene_prog, "view_matrix");
	render_scene_uniforms.projection_matrix = glGetUniformLocation(render_scene_prog, "projection_matrix");
	render_scene_uniforms.aspect = glGetUniformLocation(render_scene_prog, "aspect");
	render_scene_uniforms.time = glGetUniformLocation(render_scene_prog, "time");

	BlitShader.init();
	BlitShader.attach(GL_VERTEX_SHADER, "resolve.vert");
	BlitShader.attach(GL_FRAGMENT_SHADER, "resolve.frag");
	BlitShader.link();
	resolve_program = BlitShader.GetProgram();
}





