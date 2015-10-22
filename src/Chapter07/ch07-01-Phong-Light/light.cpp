#include "light.h"

namespace byhj
{

void Light::init_buffer()
{
	object.LoadFromVBM("../../../media/objects/torus.vbm", 0, 1, 2);
}

void Light::init_vertexArray()
{

}


void Light::init_shader()
{
	ObjectShader.init();
	ObjectShader.attach(GL_VERTEX_SHADER, "light.vert");
	ObjectShader.attach(GL_FRAGMENT_SHADER, "light.frag");
	ObjectShader.link();
	ObjectShader.info();

	program = ObjectShader.GetProgram();

	uniformLoc.mv = glGetUniformLocation(program, "model");
	uniformLoc.proj = glGetUniformLocation(program, "proj");
	uniformLoc.color_ambient = glGetUniformLocation(program, "color_ambient");
	uniformLoc.color_diffuse = glGetUniformLocation(program, "color_diffuse");
	uniformLoc.color_specular = glGetUniformLocation(program, "color_specular");
}


void Light::init_texture()
{
	tex = loadTexture("../../../media/textures/test.dds");
}

}
