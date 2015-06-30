#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ogl/vbm.h"
#include "ogl/oglShader.h"


const int  INSTANCE_COUNT = 200 ; 

class Instance
{

public:
	Instance(): InstanceShader("Instance Shader") {}
	~Instance() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
		// Setup
	   glEnable(GL_CULL_FACE);
	   glEnable(GL_DEPTH_TEST); 
	   glDepthFunc(GL_LEQUAL);

       init_buffer();
	   init_vertexArray();
	   init_shader();
	}

	/////////////////////////////Render/////////////////////////////
	void Render(float aspect)
	{
		glUseProgram(program);
		object.BindVertexArray();

		//We change view by click the mosue
		float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);

		static float q = 0.0f;
		static const glm::vec3 X(1.0f, 0.0f, 0.0f);
		static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
		static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

		// Set weights for each instance
		glm::vec4 weights[INSTANCE_COUNT];

		for (int n = 0; n < INSTANCE_COUNT; n++) 
		{
			float a = float(n) / 4.0f;
			float b = float(n) / 5.0f;
			float c = float(n) / 6.0f;
			weights[n][0] = 0.5f * (sinf(t * 6.28318531f * 8.0f + a) + 1.0f);
			weights[n][1] = 0.5f * (sinf(t * 6.28318531f * 26.0f + b) + 1.0f);
			weights[n][2] = 0.5f * (sinf(t * 6.28318531f * 21.0f + c) + 1.0f);
			weights[n][3] = 0.5f * (sinf(t * 6.28318531f * 13.0f + a + b) + 1.0f);
		}

		//We change the weight data every frame, so we should use GL_DYNAMIC_DRAW
		glBindBuffer(GL_ARRAY_BUFFER, weight_vbo);  
		glBufferData(GL_ARRAY_BUFFER, sizeof(weights), weights, GL_DYNAMIC_DRAW);

		// Set four model matrices
		static glm::mat4 model_matrix[4];
		//Ä£ÐÍ¾ØÕó
		for (int n = 0; n < 4; n++) 
		{
			model_matrix[n] = glm::scale(glm::mat4(1.0), glm::vec3(5.0f, 5.0f, 5.0f) )
				            * glm::rotate(glm::mat4(1.0), glm::radians(t * 360.0f * 40.0f + float(n + 1) * 29.0f), glm::vec3(0.0f, 1.0f, 0.0f))
				            * glm::rotate(glm::mat4(1.0), glm::radians(t * 360.0f * 20.0f + float(n + 1) * 35.0f), glm::vec3(0.0f, 0.0f, 1.0f)) 
				            * glm::rotate(glm::mat4(1.0), glm::radians(t * 360.0f * 30.0f + float(n + 1) * 67.0f), glm::vec3(0.0f, 1.0f, 0.0f)) 
				            * glm::translate(glm::mat4(1.0), glm::vec3( (float)n * 10.0f - 15.0f, 0.0f, 0.0f) )
				            * glm::scale(glm::mat4(1.0), glm::vec3(0.01f, 0.01f, 0.01f)) ;
		}
		//We send four model matrix to shader
		glUniformMatrix4fv(uniform.model_loc, 4, GL_FALSE, glm::value_ptr(model_matrix[0]));

		glm::mat4 proj = glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) 
			           * glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -50.0f)) ;
		glUniformMatrix4fv(uniform.proj_loc, 1, GL_FALSE, glm::value_ptr(proj) );

		//Draw the instance
		object.Render(0, INSTANCE_COUNT);	
	}

	void Shutdown()
	{
		glDeleteProgram(program);
	}

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();

private:

    struct Uniform
	{
		GLuint model_loc;
		GLuint proj_loc;
	};
	Uniform uniform;

	GLuint vao[2], vbo[2];
	GLuint weight_vbo, color_vbo;
	GLuint program;
	VBObject object;
	OGLShader InstanceShader;
};


void Instance::init_buffer()
{
	object.LoadFromVBM("../../../media/objects/armadillo_low.vbm", 0, 1, 2);

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

	//Weight
	glGenBuffers(1, &weight_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, weight_vbo); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Color
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Instance::init_vertexArray()
{
	object.BindVertexArray();

	//Send data each instance, not each vertex
	glBindBuffer(GL_ARRAY_BUFFER, weight_vbo);
	glVertexAttribDivisor(3, 1); 
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribDivisor(4, 1);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(4);

	// Done (unbind the object's VAO)
	glBindVertexArray(0);
}


void Instance::init_shader()
{
	InstanceShader.init();
	InstanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	InstanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	InstanceShader.link();
	program = InstanceShader.GetProgram();
	uniform.proj_loc   = glGetUniformLocation(program, "proj");
	uniform.model_loc = glGetUniformLocation(program, "model");
}

#endif


