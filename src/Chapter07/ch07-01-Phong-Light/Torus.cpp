#include "Torus.h"
#include "ogl/vmath.h"
#include <GLFW/glfw3.h>

namespace byhj
{

void Torus::Init()
{
	init_buffer();
	init_vertexArray();
	init_shader();
}

void Torus::Render(GLfloat aspect)
{
	glUseProgram(program);
	glBindVertexArray(vao);

	float time = glfwGetTime() / 100.0f;

	vmath::mat4 model = vmath::translate(0.0f, 0.0f, -4.0f)
		* vmath::rotate(987.0f * time * 3.14159f, vmath::vec3(0.0f, 0.0f, 1.0f))
		* vmath::rotate(1234.0f * time * 3.14159f, vmath::vec3(1.0f, 0.0f, 0.0f));
	vmath::mat4 view = vmath::lookat(vmath::vec3(0.0f, 0.0f, 100.0f),
		vmath::vec3(0.0f, 0.0f, 0.0f),
		vmath::vec3(0.0f, 1.0f, 0.0f));

	vmath::mat4 mv = view * model;
	vmath::mat4 prj_matrix = vmath::perspective(60.0f, aspect, 0.1f, 1000.0f);

	glUseProgram(program);
	glUniformMatrix4fv(uniformLoc.mv, 1, GL_FALSE, mv);
	glUniformMatrix4fv(uniformLoc.proj, 1, GL_FALSE, prj_matrix);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	object.Render();
}

void Torus::Shutdown()
{
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}



void Torus::init_shader()
{
	ObjectShader.init();
	ObjectShader.attach(GL_VERTEX_SHADER, "light.vert");
	ObjectShader.attach(GL_FRAGMENT_SHADER, "light.frag");
	ObjectShader.link();
	ObjectShader.info();

	program = ObjectShader.GetProgram();

	uniformLoc.mv = glGetUniformLocation(program, "model");
	uniformLoc.proj = glGetUniformLocation(program, "proj");
	uniformLoc.color_ambient = glGetUniformLocation(program, "color_ambient");
	uniformLoc.color_diffuse = glGetUniformLocation(program, "color_diffuse");
	uniformLoc.color_specular = glGetUniformLocation(program, "color_specular");
}

void Torus::init_buffer()
{
  object.LoadFromVBM("../../../media/objects/torus.vbm", 0, 1, 2);
}

void Torus::init_vertexArray()
{

}


}