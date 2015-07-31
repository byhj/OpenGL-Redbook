#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ogl/oglShader.h"
#include "ogl/oglUtility.h"

namespace byhj
{

	class Triangle
	{

	public:
		Triangle()  {}
		~Triangle() {}

	public:

		void Init();
		void Render();
		void Shutdown();


	private:
		void init_shader();
		void init_buffer();
		void init_vertexArray();

	private:

		GLuint vao = byhj::OGL_VALUE;
		GLuint ibo = byhj::OGL_VALUE;
		GLuint vbo = byhj::OGL_VALUE;
		GLuint program = byhj::OGL_VALUE;
		GLuint mvp_loc = byhj::OGL_VALUE;

		byhj::Shader TriangleShader ={ "Triangle Shader" };

		GLuint m_VertexCount = 0;
		GLuint m_IndexCount  = 0;
	};


}
#endif