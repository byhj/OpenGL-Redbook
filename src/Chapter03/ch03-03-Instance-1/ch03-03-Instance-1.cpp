#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <common/shader.h>
#include <common/vbm.cpp>

const int Width = 1000, Height = 800; 
const int  INSTANCE_COUNT = 200 ; 

// Member variables
static float aspect;
GLuint vao[2], vbo[2];
GLuint weight_vbo, color_vbo;
GLuint program;
GLint  model_loc, proj_loc;
VBObject object;
Shader instanceShader("instance Shader");

void init_shader()
{
	instanceShader.init();
	instanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	instanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	instanceShader.link();
	instanceShader.interfaceInfo();

	program = instanceShader.GetProgram(); 

	model_loc = glGetUniformLocation(program, "model");
	proj_loc= glGetUniformLocation(program, "proj");
}

void init_buffer()
{

	object.LoadFromVBM("../../../media/objects/armadillo_low.vbm", 0, 1, 2);

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

	//Weight
	glGenBuffers(1, &weight_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, weight_vbo); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Color
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init_vertexArray()
{
	object.BindVertexArray();

	glBindBuffer(GL_ARRAY_BUFFER, weight_vbo);
	//Send data each instance, not each vertex
	glVertexAttribDivisor(3, 1); 
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribDivisor(4, 1);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(4);

	// Done (unbind the object's VAO)
	glBindVertexArray(0);
}

void init(void)
{
	init_shader();
	init_buffer();
	init_vertexArray();

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

	// Set weights for each instance
	glm::vec4 weights[INSTANCE_COUNT];

	for (int n = 0; n < INSTANCE_COUNT; n++) 
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		weights[n][0] = 0.5f * (sinf(t * 6.28318531f * 8.0f + a) + 1.0f);
		weights[n][1] = 0.5f * (sinf(t * 6.28318531f * 26.0f + b) + 1.0f);
		weights[n][2] = 0.5f * (sinf(t * 6.28318531f * 21.0f + c) + 1.0f);
		weights[n][3] = 0.5f * (sinf(t * 6.28318531f * 13.0f + a + b) + 1.0f);
	}

	//We change the weight data every frame, so we should use GL_DYNAMIC_DRAW
	glBindBuffer(GL_ARRAY_BUFFER, weight_vbo);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(weights), weights, GL_DYNAMIC_DRAW);

	// Set four model matrices
	static glm::mat4 model_matrix[4];
	//Ä£ÐÍ¾ØÕó
	for (int n = 0; n < 4; n++) 
	{
		model_matrix[n] = (glm::scale(glm::mat4(1.0), glm::vec3(5.0f, 5.0f, 5.0f) ) *
			glm::rotate(glm::mat4(1.0), t * 360.0f * 40.0f + float(n + 1) * 29.0f, glm::vec3(0.0f, 1.0f, 0.0f))*
			glm::rotate(glm::mat4(1.0), t * 360.0f * 20.0f + float(n + 1) * 35.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0), t * 360.0f * 30.0f + float(n + 1) * 67.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::translate(glm::mat4(1.0), glm::vec3( (float)n * 10.0f - 15.0f, 0.0f, 0.0f) )*
			glm::scale(glm::mat4(1.0), glm::vec3(0.01f, 0.01f, 0.01f)) );
	}

	//We send four model matrix to shader
	glUniformMatrix4fv(model_loc, 4, GL_FALSE, glm::value_ptr(model_matrix[0]));

	glm::mat4 projection_matrix(glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) 
		* glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -50.0f)) );
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	//Draw the instance
	object.Render(0, INSTANCE_COUNT);	

	glutSwapBuffers();
}

int main(int argc,char ** argv)
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


