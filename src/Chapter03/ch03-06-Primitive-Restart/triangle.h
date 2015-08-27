#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ogl/oglShader.h"

const int VertexCount = 6;

class Triangle
{

public:
	Triangle():TriangleShader("Triangle Shader") {}
	~Triangle() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
	   glEnable(GL_CULL_FACE);
       init_buffer();
	   init_vertexArray();
	   init_shader();
	}

	/////////////////////////////Render/////////////////////////////
	void Render()
	{
		glUseProgram(program);
		glBindVertexArray(VAO);

		// mvp矩阵
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -5.0));
		model *= glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		//set the view and projection matrix

		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 projection = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
		glm::mat4 mvp  = projection * view;

		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//使用图元中断，条件编译
#ifdef USE_PRIMITIVE_RESTART   
		// When primitive restart is on, we can call one draw command
		glEnable(GL_PRIMITIVE_RESTART);  //开启图元中断
		glPrimitiveRestartIndex(0xFFFF); //设置中断标志
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL); //绘制函数
#else
		//不使用图元中断
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid *)(9 * sizeof(GLushort)));
#endif

	}

	void Shutdown()
	{
		glDeleteProgram(program);
	}

private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	OGLShader TriangleShader;
	GLuint program;
	GLuint modelLoc;
	GLuint mvpLoc;
	GLuint EBO, VBO, VAO;
};


void Triangle::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "cube.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	TriangleShader.link();
	program = TriangleShader.GetProgram();
	modelLoc= glGetUniformLocation(program, "model_matrix");
	mvpLoc = glGetUniformLocation(program, "mvp_matrix");
}

#define USE_PRIMITIVE_RESTART//开启图元中断

static const GLfloat positions[] = //顶点数据
{
	-1.0f, -1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f, -1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f,  1.0f, -1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

//颜色数据
static const GLfloat colors[] =
{
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f
};

// Indices for the triangle strips
static const GLushort indices[] =
{
	0, 1, 2, 3, 6, 7, 4, 5,         // First strip
	0xFFFF,                         // 重启动标志
	2, 6, 0, 4, 1, 5, 3, 7          // Second strip
};


void Triangle::init_buffer()
{

	glGenBuffers(1, &VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), NULL, GL_STATIC_DRAW);//先分配内存，不传入数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);  
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Triangle::init_vertexArray()
{

	glGenVertexArrays(1, &VAO);  
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

#endif

/*
void keyboard(unsigned char key, int x, int y)
{
	//Enter key 'm' to change the display mode
	switch(key) {
	case 'm': 
		{  
			static GLenum mode = GL_FILL;
			mode = ( mode == GL_FILL ? GL_LINE : GL_FILL); 
			glPolygonMode(GL_FRONT_AND_BACK, mode); 
		}
		break;
	}
}

*/