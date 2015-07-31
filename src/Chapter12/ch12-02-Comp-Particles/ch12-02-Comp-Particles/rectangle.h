#ifndef Rectangle_H
#define Rectangle_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

enum
{
	PARTICLE_GROUP_SIZE     = 128,
	PARTICLE_GROUP_COUNT    = 8000,
	PARTICLE_COUNT          = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT),
	MAX_ATTRACTORS          = 64
};

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

	GLuint vao = byhj::OGL_VALUE;
	GLuint vbo = byhj::OGL_VALUE;
	GLuint program = byhj::OGL_VALUE;
	byhj::Shader AppShader  = { "Rectangle Shader" };
	byhj::Shader CompShader = { "Compute Shader" };;

	GLuint  output_image;
	// Member variables
	GLuint  compute_prog;
	GLuint  render_prog;
	// Compute program
	GLint   dt_location;

	// Posisition and velocity buffers
	union
	{
		struct
		{
			GLuint position_buffer;
			GLuint velocity_buffer;
		};
		GLuint buffers[2];
	};

	// TBOs
	union
	{
		struct
		{
			GLuint position_tbo;
			GLuint velocity_tbo;
		};
		GLuint tbos[2];
	};

	// Attractor UBO
	GLuint  attractor_buffer;

	// Program, vao and vbo to render a full screen quad
	GLuint  render_vao;
	GLuint  render_vbo;

	// Mass of the attractors
	float attractor_masses[MAX_ATTRACTORS];
};


}
#endif