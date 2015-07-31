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

	//We change view by click the mosue
	float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);

	static float q = 0.0f;
	static const glm::vec3 X(1.0f, 0.0f, 0.0f);
	static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
	static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

	// Set model matrices for each instance
	glm::mat4 matrices[INSTANCE_COUNT];

	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		float a = 50.0f * float(n) / 4.0f;
		float b = 50.0f * float(n) / 5.0f;
		float c = 50.0f * float(n) / 6.0f;

		matrices[n] = glm::rotate(glm::mat4(1.0), a + t * 360.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0), b + t * 360.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0), c + t * 360.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::translate(glm::mat4(1.0), glm::vec3(10.0f + a, 40.0f + b, 50.0f + c));
	}

	glBindBuffer(GL_TEXTURE_BUFFER, model_tbo);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(matrices), matrices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

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

	//uniform Index
	view_loc = glGetUniformLocation(program, "view");
	proj_loc= glGetUniformLocation(program, "proj");

	//Vertex Attrib Array index
	color_tbo_loc = glGetUniformLocation(program, "color_tbo");
	model_tbo_loc = glGetUniformLocation(program, "model_tbo");

	glUseProgram(program);
	glUniform1i(color_tbo_loc, 0);
	glUniform1i(model_tbo_loc, 1);
}



void Instance::init_buffer()
{
	m_Armadillo.LoadFromVBM("../../../media/objects/armadillo_low.vbm", 0, 1, 2);


	m_Armadillo.BindVertexArray();

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

	//Notice the set color_tbo to gl_texture_buffer layer 0, your should use texture to change it
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &color_texture);
	glBindTexture(GL_TEXTURE_BUFFER, color_texture);

	glGenBuffers(1, &color_tbo);
	glBindBuffer(GL_TEXTURE_BUFFER, color_tbo);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, color_tbo);

	// Now do the same thing with a TBO for the model matrices. The buffer object
	// (model_matrix_buffer) has been created and sized to store one mat4 per-
	// instance.

	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &model_texture);
	glBindTexture(GL_TEXTURE_BUFFER, model_texture);

	glGenBuffers(1, &model_tbo);
	glBindBuffer(GL_TEXTURE_BUFFER, model_tbo);
	glBufferData(GL_TEXTURE_BUFFER, INSTANCE_COUNT * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, model_tbo);

}

void Instance::init_vertexArray()
{

}


}


