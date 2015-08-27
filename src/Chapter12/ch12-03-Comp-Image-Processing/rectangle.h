#ifndef Rectangle_H
#define Rectangle_H

#include "ogl/Shader.h"
#include "ogl/Utility.h"

namespace byhj
{

class Rectangle
{

public:
	Rectangle()  {}
	~Rectangle() {}

public:
				
	void Init()		;
	void Render()	;
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();


private:

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;
	ogl::Shader AppShader  = { "Rectangle Shader" };
	ogl::Shader CompShader = { "Compute Shader" };;
	GLuint  render_vao;
	GLuint  render_vbo;
	// Texture to process
	GLuint  input_image;
	// Texture for compute shader to write into
	GLuint  intermediate_image;
	GLuint  output_image;

	GLuint  compute_prog;
	GLuint  render_prog;

};


}
#endif