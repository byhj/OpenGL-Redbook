#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl/oglShader.h"
#include "ogl/vmath.h"
#include "ogl/vbm.h"

class Triangle
{

public:
	Triangle():vao(-1), vbo(-1) {}
	~Triangle() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
       init_buffer();
	   init_vertexArray();
	   init_shader();
	}

	/////////////////////////////Render/////////////////////////////
	void Render(int w, int h)
	{
		float t = float(GetTickCount() & 0x3FFF) / float(0x3FFF);
		static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
		static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
		static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

		glClearColor( 0.3f, 0.1f, 0.2f, 1.0f );
		glClearDepth( 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram(prog);
		static const float aspect = static_cast<float>(w) / static_cast<float>(h);
		vmath::mat4 p(vmath::frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 5000.0f));
		vmath::mat4 m[4];

		for (int i = 0; i < 4; i++)
		{
			m[i] = vmath::mat4(vmath::translate(0.0f, 0.0f, 100.0f * sinf(6.28318531f * t + i) - 230.0f) *
				vmath::rotate(360.0f * t * float(i + 1), X) *
				vmath::rotate(360.0f * t * float(i + 2), Y) *
				vmath::rotate(360.0f * t * float(5 - i), Z) *
				vmath::translate(0.0f, -80.0f, 0.0f));
		}		   

		//We send the four model matrix to see the different scene in different viewport
		glUniformMatrix4fv(model_matrix_pos, 4, GL_FALSE, m[0]);
		glUniformMatrix4fv(projection_matrix_pos, 1, GL_FALSE, p);

		const float wot = float(w) * 0.5f;
		const float hot = float(h) * 0.5f;
		glViewportIndexedf(0, 0.0f, 0.0f, wot, hot);
		glViewportIndexedf(1, wot, 0.0f, wot, hot);
		glViewportIndexedf(2, 0.0f, hot, wot, hot);
		glViewportIndexedf(3, wot, hot, wot, hot);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		object.Render();
	}

	void Shutdown()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:


	OGLShader ViewportShader;
	GLuint prog;
	GLuint vao;
	GLuint vbo;
	VBObject object;

	GLint model_matrix_pos;
	GLint projection_matrix_pos;
};


void Triangle::init_shader()
{
	ViewportShader.init();
	ViewportShader.attach(GL_VERTEX_SHADER, "view.vert");
	ViewportShader.attach(GL_GEOMETRY_SHADER, "view.geom");
	ViewportShader.attach(GL_FRAGMENT_SHADER, "view.frag");
	ViewportShader.link();
	ViewportShader.interfaceInfo();
	prog = ViewportShader.GetProgram();
	model_matrix_pos = glGetUniformLocation(prog, "model_matrix");
	projection_matrix_pos = glGetUniformLocation(prog, "projection_matrix");
}

void Triangle::init_buffer()
{
   object.LoadFromVBM("../../../media/objects/ninja.vbm", 0, 1, 2);
}

void Triangle::init_vertexArray()
{

}

#endif


