#ifndef Rectangle_H
#define Rectangle_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

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
	byhj::Shader AppShader  = { "Rectangle Shader" };
	byhj::Shader CompShader = { "Compute Shader" };;
	GLuint  output_image;
	// Member variables
	GLuint  compute_prog;
	GLuint  render_prog;

};


}
#endif