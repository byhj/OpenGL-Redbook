#include "Rectangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "ogl/loadTexture.h"
#include "ogl/oglRandom.h"
#include "ogl/vLoadTexture.h"
#pragma  comment (lib, "vermilion32_d.lib")

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


	static float totalTime = 0.0f;
	static float lastTime = glfwGetTime();
	float currentTime = glfwGetTime();
	totalTime += currentTime - lastTime;

	static glm::vec4 ranColor = glm::vec4(1.0f);
	glm::mat4 tc_matrix = glm::rotate(glm::mat4(1.0), glm::radians(currentTime * 17.0f), glm::vec3(1.0, 0.0, 0.0)) *
		glm::rotate(glm::mat4(1.0), glm::radians(currentTime * 13.0f), glm::vec3(0.0, 1.0, 0.0))  *
		glm::rotate(glm::mat4(1.0), glm::radians(currentTime * 9.0f), glm::vec3(0.0, 0.0, 1.0));

	glUniformMatrix4fv(tc_rotate_loc, 1, GL_FALSE, glm::value_ptr(tc_matrix));

	//We change the cloud color every second
	static byhj::OGLRandom random;
	if (totalTime > 1.0f)
	{
		ranColor = random.GetColor();
		totalTime = 0.0f;
	}
	glUniform4fv(randomColor_loc, 1, &ranColor[0]);


	glUniform1i(tex_loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, tex);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	lastTime = currentTime;

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
	RectangleShader.attach(GL_VERTEX_SHADER,   "texture.vert");
	RectangleShader.attach(GL_FRAGMENT_SHADER, "texture.frag");
	RectangleShader.link();
	program = RectangleShader.GetProgram();
	tex_loc = glGetUniformLocation(program, "tex");

	tc_rotate_loc = glGetUniformLocation(program, "tc_rotate");
	randomColor_loc = glGetUniformLocation(program, "randomColor");
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
	
	glBindVertexArray(0);
}


void Rectangle::init_texture()
{
	vglImageData image;

	tex = vglLoadTexture("../../../media/textures/cloud.dds", 0, &image);
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	vglUnloadImage(&image);
}

}