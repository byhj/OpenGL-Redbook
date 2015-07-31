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
	void Render(GLfloat aspect)	;
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();

private:

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint ibo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;
	byhj::Shader RectangleShader = { "Rectangle Shader" };
	GLuint texture, tex_loc;
	GLuint rotate_loc;
	GLuint m_VertexCount = 0;
	GLuint m_IndexCount  = 0 ;
};


}
#endif