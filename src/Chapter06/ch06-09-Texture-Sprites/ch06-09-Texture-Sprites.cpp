#include<gl/glew.h>
#include<gl/freeglut.h>
#include<iostream>
#include<cstdio>

#include <common/shader.h>
#include <common/loadTexture.h>
#include <common/vmath.h>

#define POINT_COUNT 200

const int Width = 1000, Height = 800; 
float aspect;
GLuint render_prog;
GLuint vao[1];
GLuint vbo[1];
GLuint sprite_texture;
GLint model_loc;
GLint proj_loc;

static inline float random_float()
{
    float res;
    unsigned int tmp;
    static unsigned int seed = 0x13371337;

    seed *= 16807;

    tmp = seed ^ (seed >> 4) ^ (seed << 15);

    *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}

Shader spriteShader("sprite Shader");

void load_shaders()
{
	spriteShader.init();
	spriteShader.attach(GL_VERTEX_SHADER, "pointsprites.vert");
	spriteShader.attach(GL_FRAGMENT_SHADER, "pointsprites.frag");
	spriteShader.link();
	spriteShader.use();
	render_prog = spriteShader.GetProgram();
	glUseProgram(render_prog);
	 // "model_matrix" is actually an array of 4 matrices

    model_loc = glGetUniformLocation(render_prog, "model");
    proj_loc = glGetUniformLocation(render_prog, "proj");
}

void init_buffer()
{
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, POINT_COUNT * sizeof(vmath::vec4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void init_vertexArray()
{    // Set up the vertex attributes
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	vmath::vec4 * vertex_positions = (vmath::vec4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	for (int n = 0; n < POINT_COUNT; n++)
	{
		vertex_positions[n] = vmath::vec4(random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, 1.0f);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}


void init(void) 
{
	load_shaders();
	init_buffer();
	init_vertexArray();

	sprite_texture = loadTexture("../../../media/textures/sprite.tga");
	glGenerateMipmap(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display(void)
{
    float t = glutGet(GLUT_ELAPSED_TIME) / 10000.0f;

    static float q = 0.0f;
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

    // Setup
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate simple shading program
    glUseProgram(render_prog);
	glBindVertexArray(vao[0]);

    glEnable(GL_BLEND); //开启混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set up the model and projection matrix
	vmath::mat4 proj(vmath::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 8.0f));
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj);

    // Draw Arrays...
	vmath::mat4 model = vmath::translate(0.0f, 0.0f, -2.0f) 
		              * vmath::rotate(t * 360.0f, Y) * rotate(t * 720.0f, Z);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

	glBindTexture(GL_TEXTURE_2D, sprite_texture);
	//Enable it to set shader gl_PointSize work
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SPRITE);
    glPointSize(32.0f);

    glDrawArrays(GL_POINTS, 0, POINT_COUNT); //绘制点

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
    aspect = float(width) / float(height);
}

void finalize(void)
{
    glUseProgram(0);
    glDeleteProgram(render_prog);
    glDeleteVertexArrays(1, vao);
    glDeleteBuffers(1, vbo);
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("PointSprites");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		std::cout << "Init the glew Error" << glewGetErrorString(res) << std::endl;

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	finalize();
	return 0;
}
