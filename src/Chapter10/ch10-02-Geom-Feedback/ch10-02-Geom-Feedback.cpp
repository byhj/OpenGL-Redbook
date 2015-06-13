#include <common/vbm.cpp>
#include <common/vmath.h>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <common/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader baseShader("Base Shader"), FbShader("FB Shader");
const int Width = 1200, Height = 800; 

// Member variables
float aspect;
GLuint sort_prog;
GLuint render_prog;
GLuint vao[2];
GLuint vbo[2];
GLuint xfb;
VBObject object;

GLint model_matrix_pos;
GLint projection_matrix_pos;


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
	 aspect = float(h) / float(w);
}

void display(void)
{
	float t = float(GetTickCount() & 0x3FFF) / float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(sort_prog);

	vmath::mat4 p(vmath::frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 5000.0f));
	vmath::mat4 m;

	m = vmath::mat4(vmath::translate(0.0f,
		0.0f,
		100.0f * sinf(6.28318531f * t) - 230.0f) *
		vmath::rotate(360.0f * t, X) *
		vmath::rotate(360.0f * t * 2.0f, Y) *
		vmath::rotate(360.0f * t * 5.0f, Z) *
		vmath::translate(0.0f, -80.0f, 0.0f));

	glUniformMatrix4fv(model_matrix_pos, 1, GL_FALSE, m[0]);
	glUniformMatrix4fv(projection_matrix_pos, 1, GL_FALSE, p);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
	glBeginTransformFeedback(GL_POINTS);

	object.Render();

	glEndTransformFeedback();
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	glDisable(GL_RASTERIZER_DISCARD);

	static const vmath::vec4 colors[2] =
	{
		vmath::vec4(0.8f, 0.8f, 0.9f, 0.5f),
		vmath::vec4(0.3f, 1.0f, 0.3f, 0.8f)
	};

	glUseProgram(render_prog);

	glUniform4fv(0, 1, colors[0]);
	glBindVertexArray(vao[0]);
	glDrawTransformFeedbackStream(GL_TRIANGLES, xfb, 0);

	glUniform4fv(0, 1, colors[1]);
	glBindVertexArray(vao[1]);
	glDrawTransformFeedbackStream(GL_TRIANGLES, xfb, 1);

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
	FbShader.init();
	FbShader.attach(GL_VERTEX_SHADER, "sort.vert");
	FbShader.attach(GL_GEOMETRY_SHADER, "sort.geom");
	FbShader.linkAttach();
	sort_prog = FbShader.GetProgram();

	static const char * varyings[] =
	{
		"rf_position", "rf_normal",
		"gl_NextBuffer",
		"lf_position", "lf_normal"
	};
	glTransformFeedbackVaryings(sort_prog, 5, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(sort_prog);
	glUseProgram(sort_prog);
	model_matrix_pos = glGetUniformLocation(sort_prog, "model_matrix");
	projection_matrix_pos = glGetUniformLocation(sort_prog, "projection_matrix");

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	for (int i = 0; i < 2; i++)
	{
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, vbo[i]);
		glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, 1024 * 1024 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i, vbo[i]);

		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vmath::vec4) + sizeof(vmath::vec3), NULL);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vmath::vec4) + sizeof(vmath::vec3), (GLvoid *)(sizeof(vmath::vec4)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	baseShader.init();
	baseShader.attach(GL_VERTEX_SHADER, "base.vert");
	baseShader.attach(GL_FRAGMENT_SHADER, "base.frag");
	baseShader.link();
	baseShader.use();
	render_prog = baseShader.GetProgram();
}


void init_buffer()
{
	object.LoadFromVBM("../../../media/objects/ninja.vbm", 0, 1, 2);
}

void init_vertexArray()
{
}

