#include <common/vbm.cpp>
#include <common/vmath.h>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <common/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GLuint vao, vbo, ibo, program = 0;
Shader baseShader("Base Shader"), FurShader("Fur Shader");
const int Width = 1200, Height = 800; 
GLuint model_loc, mvp_loc;

// Member variables
float aspect;
GLuint base_prog;
GLuint fur_prog;
GLuint fur_texture;
VBObject object;

GLint fur_model_matrix_pos;
GLint fur_projection_matrix_pos;
GLint base_model_matrix_pos;
GLint base_projection_matrix_pos;

void init_shader();
void init_buffer();
void init_vertexArray();

void init(void)
{
	init_shader();
	init_buffer();
	init_vertexArray();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //设置颜色缓存为黑色
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //视口变换

}

void display(void)
{
	//Clear the framebuffer
	glClear(GL_COLOR_BUFFER_BIT); 

	float t = float(GetTickCount() & 0x3FFF) / float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vmath::mat4 p(vmath::frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 5000.0f));
	vmath::mat4 m = vmath::mat4(vmath::translate(0.0f,
		                                         0.0f,
		                                         /* 100.0f * sinf(6.28318531f * t)*/ - 130.0f) *
		                                         //vmath::rotation(360.0f * t, X) *
		                                         vmath::rotate(360.0f * t * 1.0f, Y) *
		                                         vmath::rotate(180.0f, Z) *
		                                         vmath::translate(0.0f, -80.0f, 0.0f));

	glUseProgram(base_prog);
	object.BindVertexArray();
	glUniformMatrix4fv(base_model_matrix_pos, 1, GL_FALSE, m[0]);
	glUniformMatrix4fv(base_projection_matrix_pos, 1, GL_FALSE, p);

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	object.Render();

	/////////////////////////////////////////////////////////////
	glUseProgram(fur_prog);

	glUniformMatrix4fv(fur_model_matrix_pos, 1, GL_FALSE, m[0]);
	glUniformMatrix4fv(fur_projection_matrix_pos, 1, GL_FALSE, p);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);

	object.Render();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glutSwapBuffers();
	glutPostRedisplay();
}


int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Triangle");

	//glewInit() use first in order using other function
	GLenum res = glewInit(); 
	if (res != GLEW_OK) 
		std::cout << "Init the glew Error" << glewGetErrorString(res) << std::endl;

	init();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	glutMainLoop();    
	return 0;
}


void init_shader()
{
	baseShader.init();
	baseShader.attach(GL_VERTEX_SHADER, "base.vert");
	baseShader.attach(GL_FRAGMENT_SHADER, "base.frag");
	baseShader.link();
	baseShader.interfaceInfo();
	base_prog = baseShader.GetProgram();
	base_model_matrix_pos = glGetUniformLocation(base_prog, "model_matrix");
	base_projection_matrix_pos = glGetUniformLocation(base_prog, "projection_matrix");

	FurShader.init();
	FurShader.attach(GL_VERTEX_SHADER, "fur.vert");
	FurShader.attach(GL_GEOMETRY_SHADER, "fur.geom");
	FurShader.attach(GL_FRAGMENT_SHADER, "fur.frag");
    FurShader.link();
	FurShader.interfaceInfo();
	fur_prog = FurShader.GetProgram();
	fur_model_matrix_pos = glGetUniformLocation(fur_prog, "model_matrix");
	fur_projection_matrix_pos = glGetUniformLocation(fur_prog, "projection_matrix");
}


void init_buffer()
{

	glGenTextures(1, &fur_texture);
	unsigned char * tex = (unsigned char *)malloc(1024 * 1024 * 4);
	memset(tex, 0, 1024 * 1024 * 4);
	int n, m;
	for (n = 0; n < 256; n++)
	{
		for (m = 0; m < 1270; m++)
		{
			int x = rand() & 0x3FF;
			int y = rand() & 0x3FF;
			tex[(y * 1024 + x) * 4 + 0] = (rand() & 0x3F) + 0xC0;
			tex[(y * 1024 + x) * 4 + 1] = (rand() & 0x3F) + 0xC0;
			tex[(y * 1024 + x) * 4 + 2] = (rand() & 0x3F) + 0xC0;
			tex[(y * 1024 + x) * 4 + 3] = n;
		}
	}

	glBindTexture(GL_TEXTURE_2D, fur_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	free(tex);

	object.LoadFromVBM("../../../media/objects/ninja.vbm", 0, 1, 2);
}

void init_vertexArray()
{
}

