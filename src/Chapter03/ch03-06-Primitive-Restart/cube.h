#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ogl/Shader.h"
#include "ogl/Utility.h"

namespace byhj
{

class Cube
{

public:
	Cube()  {}
	~Cube() {}

public:
				
	void Init()		;
	void Render()	;
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	GLuint modelLoc;
	GLuint mvpLoc;
	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint ibo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;
	ogl::Shader TriangleShader = { "Cube Shader" };

	GLuint m_VertexCount = 0;
	GLuint m_IndexCount  = 0 ;
};


}
#endif