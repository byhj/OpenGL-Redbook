#include "Rectangle.h"
#include "ogl/vbm.h"
#include "ogl/vmath.h"
#include "ogl/loadTexture.h"
#include <GLFW/glfw3.h>

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

	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}


namespace byhj
{

void Rectangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Rectangle::Render( GLfloat aspect)
{
	glUseProgram(program);
	glBindVertexArray(vao);


	float t = glfwGetTime() / 10.0f;

	static float q = 0.0f;
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);



	vmath::mat4 proj(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 8.0f));
	vmath::mat4 model = vmath::translate(0.0f, 0.0f, -2.0f)
		* vmath::rotate(t * 360.0f, Y) * rotate(t * 720.0f, Z);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj);

	glUniform1i(tex_loc, 0);
	glBindTexture(GL_TEXTURE_2D, sprite_texture);

	//Enable it to set shader gl_PointSize work
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SPRITE);
	glPointSize(32.0f);

	glDrawArrays(GL_POINTS, 0, POINT_COUNT); 

	glBindVertexArray(0);
	glUseProgram(program);
}

void Rectangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}



void Rectangle::init_shader()
{
	RectangleShader.init();
	RectangleShader.attach(GL_VERTEX_SHADER, "pointsprites.vert");
	RectangleShader.attach(GL_FRAGMENT_SHADER, "pointsprites.frag");
	RectangleShader.link();
	program = RectangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program,   "tex");
	model_loc = glGetUniformLocation(program, "model");
	proj_loc = glGetUniformLocation(program,  "proj");
}

void Rectangle::init_buffer()
{

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, POINT_COUNT * sizeof(vmath::vec4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Rectangle::init_vertexArray()
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


void Rectangle::init_texture()
{
	sprite_texture = loadTexture("../../../media/textures/sprite.tga");
	glGenerateMipmap(GL_TEXTURE_2D);
}

}