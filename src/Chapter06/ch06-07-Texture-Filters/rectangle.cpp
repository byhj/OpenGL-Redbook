#include "Rectangle.h"
#include "ogl/vmath.h"


namespace byhj
{

void Rectangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Rectangle::Render(GLfloat aspect)
{
	glUseProgram(program);
	glBindVertexArray(vao);


	static const unsigned int start_time = GetTickCount();
	float t = float((GetTickCount() - start_time)) / float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);


	vmath::mat4 tc_matrix(vmath::mat4::identity());
	tc_matrix = vmath::translate(vmath::vec3(0.0f, 0.0f, -60.0f)) * vmath::rotate(80.0f * 3.0f * 0.03f, X);
	tc_matrix = vmath::perspective(35.0f, 1.0f / aspect, 0.1f, 700.0f) * tc_matrix;
	glUniformMatrix4fv(rotate_loc, 1, GL_FALSE, tc_matrix);

	glUniform1i(tex_loc, 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	t = fmodf(t, 1.0f);
	if (t < 0.25f)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (t < 0.5f)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (t < 0.75f)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));

	glUseProgram(0);
	glBindVertexArray(0);
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
	RectangleShader.attach(GL_VERTEX_SHADER,   "texture.vert");
	RectangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	RectangleShader.link();
	program = RectangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
	rotate_loc = glGetUniformLocation(program, "tc_rotate");
}


static const GLfloat plane_vertices[] =
{
	-20.0f, 0.0f, -50.0f,
	-20.0f, 0.0f, 50.0f,
	20.0f, 0.0f, -50.0f,
	20.0f, 0.0f, 50.0f
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

void Rectangle::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices)+sizeof(plane_texcoords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plane_vertices), plane_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(plane_vertices), sizeof(plane_texcoords), plane_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indices), plane_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Rectangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(plane_vertices)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Rectangle::init_texture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, 7, GL_RGBA8, 64, 64);

	unsigned int * data = new unsigned int[64 * 64];
	unsigned int colors[] ={ 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF00FFFF,
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

	delete[] data;
}

}