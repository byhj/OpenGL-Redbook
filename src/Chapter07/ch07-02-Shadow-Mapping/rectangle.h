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
	void Render(float current_width, float current_height, float aspect);
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();
	void init_texture();
	void init_fbo();
	void DrawScene(bool depth_only);

private:

	GLint current_width;
	GLint current_height;
	VBObject g_object;

	byhj::Shader ObjectShader;
	byhj::Shader ShadowShader;
	GLuint ground_vao, ground_vbo;
	GLuint base_prog;

	// Member variables
	float aspect;
	// Program to render from the light's position
	GLuint shadow_prog;
	GLuint shadow_mvp_loc;
	GLuint depth_fbo, depth_texture;


	struct
	{
		GLuint model;
		GLuint view;
		GLuint proj;
		GLuint shadow_matrix;
		GLuint lightPos;
		GLuint mat_ambient;
		GLuint mat_diffuse;
		GLuint mat_specular;
		GLuint mat_specular_power;
	}base_uniform_loc;


};


}
#endif