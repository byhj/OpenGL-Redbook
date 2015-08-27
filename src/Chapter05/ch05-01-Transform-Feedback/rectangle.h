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
	void Render(GLfloat aspect);
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	ogl::Shader baseShader, FbShader;

	// Member variables
	float aspect;
	GLuint update_prog;
	GLuint vao[2];
	GLuint vbo[2];
	GLuint xfb;

	GLuint render_prog;
	GLuint geometry_vbo;
	GLuint render_vao;
	GLint render_model_matrix_loc;
	GLint render_projection_matrix_loc;

	GLuint geometry_tex;

	GLuint geometry_xfb;
	GLuint particle_xfb;

	GLint model_matrix_loc;
	GLint projection_matrix_loc;
	GLint triangle_count_loc;
	GLint time_step_loc;

	VBObject object;
};


}
#endif