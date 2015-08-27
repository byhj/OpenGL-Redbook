#ifndef Torus_H
#define Torus_H

#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "ogl/vbm.h"

namespace byhj
{

class Torus
{

public:
	Torus()  {}
	~Torus() {}

public:
				
	void Init()		;
	void Render(GLfloat aspect)	;
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;
	ogl::Shader ObjectShader ={ "Torus Shader" };
	VBObject object;

	struct UniformLoc
	{
		GLuint mv;
		GLuint proj;
		GLuint color_ambient;
		GLuint color_diffuse;
		GLuint color_specular;
	};
	UniformLoc uniformLoc;
};


}
#endif