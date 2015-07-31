#include "Instance.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int INSTANCE_COUNT = 200;

namespace byhj
{

void Instance::Init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
}

void Instance::Render(GLfloat aspect)
{
	glUseProgram(program);

	//We change view by click the mouse
	float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);

	static float q = 0.0f;
	static const glm::vec3 X(1.0f, 0.0f, 0.0f);
	static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
	static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

	//We map the model data to buffer, which opengl manage;
	glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
	glm::mat4 *model_matrix = (glm::mat4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		float a = 50.0f * float(n) / 4.0f;
		float b = 50.0f * float(n) / 5.0f;
		float c = 50.0f * float(n) / 6.0f;

		model_matrix[n] = glm::rotate(glm::mat4(1.0), a + t * 360.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0), b + t * 360.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0), c + t * 360.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::translate(glm::mat4(1.0), glm::vec3(10.0f + a, 40.0f + b, 50.0f + c));
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);


	glm::mat4 projection_matrix(glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f));
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glm::mat4 view_matrix(glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1500.0f))
		* glm::rotate(glm::mat4(1.0), t * 360.0f * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	//Draw the instance
	m_Armadillo.Render(0, INSTANCE_COUNT);

	glUseProgram(0);

}

void Instance::Shutdown()
{
	glDeleteProgram(program);
}



void Instance::init_shader()
{
	InstanceShader.init();
	InstanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	InstanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	InstanceShader.link();
	program = InstanceShader.GetProgram();

	view_loc = glGetUniformLocation(program, "view");
	proj_loc= glGetUniformLocation(program, "proj");

	color_loc = glGetAttribLocation(program, "color");
	model_loc = glGetAttribLocation(program, "model");
}



void Instance::init_buffer()
{
	m_Armadillo.LoadFromVBM("../../../media/objects/armadillo_low.vbm", 0, 1, 2);


	glm::vec4 colors[INSTANCE_COUNT];
	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		colors[n][0] = 0.5f * (sinf(a + 1.0f) + 1.0f);
		colors[n][1] = 0.5f * (sinf(b + 2.0f) + 1.0f);
		colors[n][2] = 0.5f * (sinf(c + 3.0f) + 1.0f);
		colors[n][3] = 1.0f;
	}

	//Color
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Weight
	glGenBuffers(1, &model_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
	glBufferData(GL_ARRAY_BUFFER, INSTANCE_COUNT * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Instance::init_vertexArray()
{

	m_Armadillo.BindVertexArray();
	
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribDivisor(color_loc, 1);
	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(color_loc);

	//Because mat is 4 * vec4, so we should use four attribArray
	glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
	for (int i = 0; i != 4; ++i)
	{
		glEnableVertexAttribArray(model_loc + i);
		glVertexAttribDivisor(model_loc + i, 1);
		glVertexAttribPointer(model_loc + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
	}

	// Done (unbind the object's VAO)
	glBindVertexArray(0);
}


}


