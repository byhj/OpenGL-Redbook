


#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"
#include "ogl/vbm.h"

namespace byhj
{

	class Instance
	{

	public:
		Instance()  {}
		~Instance() {}

	public:

		void Init();
		void Render(GLfloat aspect);
		void Shutdown();


	private:
		void init_shader();
		void init_buffer();
		void init_vertexArray();

	private:


	    GLuint vao[2], color_tbo, model_tbo;
	    GLint  proj_loc, view_loc;
	    GLuint color_tbo_loc,  model_tbo_loc;
	    GLuint color_texture, model_texture;
	    
	    GLuint program = byhj::OGL_VALUE;
	    byhj::Shader InstanceShader ={ "Instance Shader" };
	    
	    VBObject m_Armadillo;
	};


}
#endif





