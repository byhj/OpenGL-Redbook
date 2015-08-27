#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ogl/Shader.h"
#include "ogl/Utility.h"
#include "vbm.h"

namespace byhj
{

class Triangle
{

public:
	Triangle()  {}
	~Triangle() {}

public:
				
	void Init()		;
	void Render()	;
	void Shutdown()	;


private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:

	VBObject object;
	ogl::Shader AppShader;
	ogl::Shader BlitShader;

	// Program to construct the linked list (renders the transparent objects)
	GLuint  list_build_program;

	// Color palette buffer texture
	GLuint  image_palette_buffer;
	GLuint  image_palette_texture;

	// Output image and PBO for clearing it
	GLuint  output_texture;
	GLuint  output_texture_clear_buffer;

	// Program to render the scene
	GLuint render_scene_prog;
	struct
	{
		GLint aspect;
		GLint time;
		GLint model_matrix;
		GLint view_matrix;
		GLint projection_matrix;
	} render_scene_uniforms;

	// Program to resolve 
	GLuint resolve_program;

	GLint current_width;
	GLint current_height;
	float aspect;
	// Full Screen Quad
	GLuint  quad_vbo;
	GLuint  quad_vao;
};


}
#endif