#ifndef Rectangle_H
#define Rectangle_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
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
	void init_texture();

private:

	GLuint skybox_prog, object_prog;
	GLuint vao, base_prog;
	GLuint cube_vbo, cube_ibo;
	GLuint tex, tex_loc;
	GLint skybox_rotate_loc, proj_loc, mv_loc;
	VBObject object;
	byhj::Shader skyboxShader;
	byhj::Shader objectShader;
};


}
#endif