#ifndef Rectangle_H
#define Rectangle_H

#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/vbm.h"
namespace byhj
{

class Rectangle
{

public:
	Rectangle()  {}
	~Rectangle() {}

public:
				
	void Init()		;
	void Render(GLfloat aspect)	;
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	ogl::Shader baseShader, FbShader;

	GLuint sort_prog;
	GLuint render_prog;
	GLuint vao[2];
	GLuint vbo[2];
	GLuint xfb;
	VBObject object;

	GLint model_matrix_pos;
	GLint projection_matrix_pos;
};


}
#endif