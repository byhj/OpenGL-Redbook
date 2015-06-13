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
GLuint update_prog;
GLuint vao[2];
GLuint vbo[2];
GLuint xfb;

GLuint render_prog;
GLuint geometry_vbo;
GLuint render_vao;
GLint render_model_matrix_loc;
GLint render_projection_matrix_loc;

GLuint geometry_tex;

GLuint geometry_xfb;
GLuint particle_xfb;

GLint model_matrix_loc;
GLint projection_matrix_loc;
GLint triangle_count_loc;
GLint time_step_loc;

VBObject object;

const int point_count = 5000;
static unsigned int seed = 0x13371337;

static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

static vmath::vec3 random_vector(float minmag = 0.0f, float maxmag = 1.0f)
{
	vmath::vec3 randomvec(random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f);
	randomvec = normalize(randomvec);
	randomvec *= (random_float() * (maxmag - minmag) + minmag);

	return randomvec;
}


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

static inline int min(int a, int b)
{
	return a < b ? a : b;
}

void display(void)
{
	static int frame_count = 0;
	float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);
	static float q = 0.0f;
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

	vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f) * vmath::translate(0.0f, 0.0f, -100.0f));
	vmath::mat4 model_matrix(vmath::scale(0.3f) *
		vmath::rotate(t * 360.0f, 0.0f, 1.0f, 0.0f) *
		vmath::rotate(t * 360.0f * 3.0f, 0.0f, 0.0f, 1.0f));

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(render_prog);
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glUniformMatrix4fv(render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

	glBindVertexArray(render_vao);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, geometry_vbo);

	glBeginTransformFeedback(GL_TRIANGLES);
	object.Render();
	glEndTransformFeedback();

	glUseProgram(update_prog);
	model_matrix = vmath::mat4::identity();
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, model_matrix);
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, projection_matrix);
	glUniform1i(triangle_count_loc, object.GetVertexCount() / 3);

	if (t > q)
	{
		glUniform1f(time_step_loc, (t - q) * 2000.0f);
	}

	q = t;

	if ((frame_count & 1) != 0)
	{
		glBindVertexArray(vao[1]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[0]);
	}
	else
	{
		glBindVertexArray(vao[0]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[1]);
	}

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, min(point_count, (frame_count >> 3)));
	glEndTransformFeedback();

	glBindVertexArray(0);

	frame_count++;

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
	FbShader.attach(GL_VERTEX_SHADER, "update.vert");
	FbShader.attach(GL_FRAGMENT_SHADER, "update.frag");
	FbShader.linkAttach();
	update_prog = FbShader.GetProgram();

	static const char * varyings[] =
	{
		"position_out", "velocity_out"
	};

	glTransformFeedbackVaryings(update_prog, 2, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(update_prog);
	glUseProgram(update_prog);
	model_matrix_loc = glGetUniformLocation(update_prog, "model_matrix");
	projection_matrix_loc = glGetUniformLocation(update_prog, "projection_matrix");
	triangle_count_loc = glGetUniformLocation(update_prog, "triangle_count");
	time_step_loc = glGetUniformLocation(update_prog, "time_step");


	baseShader.init();
	baseShader.attach(GL_VERTEX_SHADER, "base.vert");
	baseShader.attach(GL_FRAGMENT_SHADER, "base.frag");
	baseShader.linkAttach();
	render_prog = baseShader.GetProgram();

	static const char * varyings2[] =
	{
		"world_space_position"
	};

	glTransformFeedbackVaryings(render_prog, 1, varyings2, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(render_prog);
	glUseProgram(render_prog);

	render_model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	for (INT i = 0; i < 2; i++)
	{
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, vbo[i]);
		glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, point_count * (sizeof(vmath::vec4) + sizeof(vmath::vec3)), NULL, GL_DYNAMIC_COPY);
		if (i == 0)
		{
			struct buffer_t {
				vmath::vec4 position;
				vmath::vec3 velocity;
			} * buffer = (buffer_t *)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_WRITE_ONLY);

			for (int j = 0; j < point_count; j++)
			{
				buffer[j].velocity = random_vector();
				buffer[j].position = vmath::vec4(buffer[j].velocity + vmath::vec3(-0.5f, 40.0f, 0.0f), 1.0f);
				buffer[j].velocity = vmath::vec3(buffer[j].velocity[0], buffer[j].velocity[1] * 0.3f, buffer[j].velocity[2] * 0.3f);
			}

			glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
		}

		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vmath::vec4) + sizeof(vmath::vec3), NULL);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vmath::vec4) + sizeof(vmath::vec3), (GLvoid *)sizeof(vmath::vec4));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	glGenBuffers(1, &geometry_vbo);
	glGenTextures(1, &geometry_tex);
	glBindBuffer(GL_TEXTURE_BUFFER, geometry_vbo);
	glBufferData(GL_TEXTURE_BUFFER, 1024 * 1024 * sizeof(vmath::vec4), NULL, GL_DYNAMIC_COPY);
	glBindTexture(GL_TEXTURE_BUFFER, geometry_tex);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, geometry_vbo);

	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);
	glBindBuffer(GL_ARRAY_BUFFER, geometry_vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

}


void init_buffer()
{
	object.LoadFromVBM("../../../media/objects/armadillo_low.vbm", 0, 1, 2);
}

void init_vertexArray()
{
}




