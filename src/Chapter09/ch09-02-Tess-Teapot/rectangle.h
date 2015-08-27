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
	void Render(GLfloat)	;
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();


private:

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint ibo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;

	GLuint  Inner_loc;  // Inner tessellation paramter
	GLuint  Outer_loc;  // Outer tessellation paramter
	GLfloat  Inner = 1.0f;
	GLfloat  Outer = 1.0f;
	ogl::Shader RectangleShader = { "Rectangle Shader" };
};


}
#endif