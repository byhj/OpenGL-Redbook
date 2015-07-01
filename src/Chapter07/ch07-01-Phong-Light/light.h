#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <GLFW//glfw3.h>

#include "ogl/oglShader.h"
#include "ogl/loadTexture.h"
#include "ogl/vmath.h"
#include "ogl/vbm.h"

class Light
{

public:
	Light():vao(-1), vbo(-1), ibo(-1), program(-1) {}
	~Light() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
       init_buffer();
	   init_vertexArray();
	   init_shader();
	   init_texture();
	}

	/////////////////////////////Render/////////////////////////////
	void Render(float aspect)
	{
		glUseProgram(program);
		glBindVertexArray(vao);  

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime() / 100.0f;

		vmath::mat4 model = vmath::translate(0.0f, 0.0f, -4.0f) 
			              * vmath::rotate(987.0f * time * 3.14159f, vmath::vec3(0.0f, 0.0f, 1.0f)) 
			              * vmath::rotate(1234.0f * time * 3.14159f, vmath::vec3(1.0f, 0.0f, 0.0f));
		vmath::mat4 view = vmath::lookat(vmath::vec3(0.0f, 0.0f, 100.0f), 
			                             vmath::vec3(0.0f, 0.0f, 0.0f), 
			                             vmath::vec3(0.0f, 1.0f, 0.0f) );

		vmath::mat4 mv = view * model;
		vmath::mat4 prj_matrix = vmath::perspective(60.0f, aspect, 0.1f, 1000.0f);

		glUseProgram(program);
		glUniformMatrix4fv(uniformLoc.mv, 1, GL_FALSE, mv);
		glUniformMatrix4fv(uniformLoc.proj, 1, GL_FALSE, prj_matrix);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		object.Render();
	}

	void Shutdown()
	{
		glDeleteProgram(program);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();

private:

	GLuint tex, tex_loc;
	GLuint vao, vbo, ibo, program;
	OGLShader ObjectShader;
	VBObject object;

	struct UniformLoc
	{
		GLuint mv;
		GLuint proj;
		GLuint color_ambient;
		GLuint color_diffuse;
		GLuint color_specular;
	};
	UniformLoc uniformLoc;

};



void Light::init_buffer()
{
	object.LoadFromVBM("../../../media/objects/torus.vbm", 0, 1, 2);
}

void Light::init_vertexArray()
{
	
}


void Light::init_shader()
{
	ObjectShader.init();
	ObjectShader.attach(GL_VERTEX_SHADER, "light.vert");
	ObjectShader.attach(GL_FRAGMENT_SHADER, "light.frag");
	ObjectShader.link();
	ObjectShader.interfaceInfo();

	program = ObjectShader.GetProgram();

	uniformLoc.mv = glGetUniformLocation(program, "model");
	uniformLoc.proj = glGetUniformLocation(program, "proj");
	uniformLoc.color_ambient = glGetUniformLocation(program, "color_ambient");
	uniformLoc.color_diffuse = glGetUniformLocation(program, "color_diffuse");
	uniformLoc.color_specular = glGetUniformLocation(program, "color_specular");
}


void Light::init_texture()
{
   tex = loadTexture("../../../media/textures/test.dds");
}

#endif
