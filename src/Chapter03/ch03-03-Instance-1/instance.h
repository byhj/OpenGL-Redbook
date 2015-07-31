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
		struct Uniform
		{
			GLuint model_loc;
			GLuint proj_loc;
		};
		Uniform uniform;


		GLuint weight_vbo, color_vbo;
		GLuint program = byhj::OGL_VALUE;
		GLuint mvp_loc = byhj::OGL_VALUE;

		byhj::Shader InstanceShader ={ "Instance Shader" };

		VBObject m_Armadillo;
	};


}
#endif