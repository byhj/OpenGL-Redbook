#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/vbm.h"

namespace byhj
{

class Triangle
{

public:
	Triangle()  {}
	~Triangle() {}

public:
				
	void Init()		;
	void Render(int w, int h);
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;
	ogl::Shader ViewportShader = { "Triangle Shader" };

	VBObject object;

	GLint model_matrix_pos;
	GLint projection_matrix_pos;
};


}
#endif