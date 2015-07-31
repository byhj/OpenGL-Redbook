#include "Rectangle.h"
#include <glm/glm.hpp>
#include "ogl/loadTexture.h"

const int VertexCount = 6;

namespace byhj
{

void Rectangle::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
	init_texture();
}

void Rectangle::Render()
{
	glUseProgram(program);
	glBindVertexArray(vao);

	glBindTexture(GL_TEXTURE_2D, tex);
	//Change the texture wrap mode 
	static const GLenum wrap_modes[] =
	{
		GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_REPEAT, GL_MIRRORED_REPEAT,
	};
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_modes[wrap_index % 4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_modes[wrap_index % 4]);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
	glUseProgram(program);
}

void Rectangle::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}


void Rectangle::ChangeTexWrap()
{
	++wrap_index;
}

void Rectangle::init_shader()
{
	RectangleShader.init();
	RectangleShader.attach(GL_VERTEX_SHADER,   "texture.vert");
	RectangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	RectangleShader.link();
	program = RectangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");
}

void Rectangle::init_buffer()
{
	m_VertexCount = VertexCount;
	static const GLfloat VertexData[] =
	{
		0.75f, -0.75f,
		-0.75f, -0.75f,
		-0.75f, 0.75f,
		0.75f, 0.75f,

		0.0f, 0.0f,
		4.0f, 0.0f,
		4.0f, 4.0f,
		0.0f, 4.0f,
	};
	static const GLsizei VertexSize = sizeof(VertexData);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Rectangle::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(GLfloat)+NULL));
	
	glBindVertexArray(0);
}

#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
static const GLubyte tex_data[] =  
{
	B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B,
	B, W, B, W, B, W, B, W,
	W, B, W, B, W, B, W, B,
};
#undef B
#undef W

void Rectangle::init_texture()
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA32F, 8, 8);

	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0, 0,
		8, 8,
		GL_RGBA, GL_UNSIGNED_BYTE,
		tex_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//We set the border color for wrapping
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &glm::vec4(0.5f, 0.0f, 0.5f, 1.0f)[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

}