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
	init_texture();
}

void Rectangle::Render()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
	glUniform1i(tex_loc, 0);
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
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
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
}

#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
static const GLubyte tex_data[] =  //棋盘黑白纹理
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
	
	GLfloat test_tex[] =
	{
		1.0f, 0.0f, 0.0f,  //red
		0.0f, 1.0f, 0.0f,  //green
		0.0f, 0.0f, 1.0f,  //blue
		1.0f, 0.0f, 1.0f,  //pink
	};

	//内部颜色格式是显卡内格式，即要显示的颜色，在测试出传入rgb分量，显示时如果设置为GL_RG32F，则考虑RG分量,外部格式中B分量被忽略
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, 2, 2);

	//外部颜色格式是数据传入表示的格式，即规定了数据怎么排列组成颜色分量,由外部格式和数据类型决定
	//在test_tex中传入四个颜色值，每三个float组成一个片元的颜色值，对应rgb分量, 
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0, 0,
		2, 2,
		GL_RGB, GL_FLOAT,
		test_tex);
	/*
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 8, 8);
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0, 0,
		8, 8,
		GL_RGBA, GL_UNSIGNED_BYTE,
		tex_data);
		*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

}

}