#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <common/shader.h>
#include <common/vbm.cpp>

const int Width = 1000, Height = 800; 
const int  INSTANCE_COUNT = 200; 

// Member variables
static float aspect;
GLuint vao[2], color_tbo, model_tbo;
GLuint program;
GLint  proj_loc, view_loc;
VBObject object;
Shader instanceShader("instance Shader");
GLuint color_tbo_loc,  model_tbo_loc;

void init_shader()
{
	instanceShader.init();
	instanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	instanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	instanceShader.link();
	instanceShader.interfaceInfo();

	program = instanceShader.GetProgram(); 
	//uniform Index
	view_loc = glGetUniformLocation(program, "view");
	proj_loc= glGetUniformLocation(program, "proj");

	//Vertex Attrib Array index
	color_tbo_loc = glGetUniformLocation(program, "color_tbo");
	model_tbo_loc = glGetUniformLocation(program, "model_tbo");

	//Set unform to Texturei. i is the value
	glUniform1i(color_tbo_loc, 0); 
	glUniform1i(model_tbo_loc, 1);
}

void init_buffer()
{

	object.LoadFromVBM("../../../media/objects/armadillo_low.vbm", 0, 1, 2);
	object.BindVertexArray();

	glm::vec4 colors[INSTANCE_COUNT];
	for (int n = 0; n < INSTANCE_COUNT; n++) 
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		colors[n][0] = 0.5f * (sinf(a + 1.0f) + 1.0f);
		colors[n][1] = 0.5f * (sinf(b + 2.0f) + 1.0f);
		colors[n][2] = 0.5f * (sinf(c + 3.0f) + 1.0f);
		colors[n][3] = 1.0f;
	}

	//You should active texture before glbufferData???
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	glGenTextures(1, &color_tbo);
	glBindTexture(GL_TEXTURE_BUFFER, color_tbo);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, color_tbo);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	glActiveTexture(GL_TEXTURE1);
	glGenBuffers(1, &model_tbo);
	glBindBuffer(GL_TEXTURE_BUFFER, model_tbo);
	glBufferData(GL_TEXTURE_BUFFER, INSTANCE_COUNT * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, model_tbo);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

}


void init(void)
{
	init_shader();
	init_buffer();

	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClearDepth(1.0f);

	// Setup
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	aspect = float(w) / float(h);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	object.BindVertexArray();

	//We change view by click the mosue
	float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);

	static float q = 0.0f;
	static const glm::vec3 X(1.0f, 0.0f, 0.0f);
	static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
	static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

	// Set model matrices for each instance
	glm::mat4 matrices[INSTANCE_COUNT];

		for (int n = 0; n < INSTANCE_COUNT; n++) 
		{
			float a = 50.0f * float(n) / 4.0f;
			float b = 50.0f * float(n) / 5.0f;
			float c = 50.0f * float(n) / 6.0f;

			matrices[n] = glm::rotate(glm::mat4(1.0), a + t * 360.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				          glm::rotate(glm::mat4(1.0), b + t * 360.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
				          glm::rotate(glm::mat4(1.0), c + t * 360.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
				          glm::translate(glm::mat4(1.0), glm::vec3(10.0f + a, 40.0f + b, 50.0f + c) );
		}
	
     glBindBuffer(GL_TEXTURE_BUFFER, model_tbo);
	 glBufferData(GL_TEXTURE_BUFFER, sizeof(matrices), matrices, GL_DYNAMIC_DRAW);
	 glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glm::mat4 projection_matrix(glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) );           
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	glm::mat4 view_matrix(glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1500.0f)) 
		* glm::rotate(glm::mat4(1.0), t * 360.0f * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) );
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));


	object.Render(0, INSTANCE_COUNT);	

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA);
		glutInitWindowPosition(300, 0);
		glutInitWindowSize(Width, Height);
		////glutInitContextVersion(4, 3);
		////glutInitContextProfile(GLUT_CORE_PROFILE);
		glutCreateWindow("Instance Draw");

		GLenum res = glewInit();  
		if (res != GLEW_OK) 
			std::cout << "Init the glew Error" << glewGetErrorString(res) << std::endl;

		init();
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutMainLoop();
		return 0;
}
