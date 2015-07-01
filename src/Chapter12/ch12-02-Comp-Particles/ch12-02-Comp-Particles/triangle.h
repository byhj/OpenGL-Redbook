#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "ogl/oglShader.h"
#include "ogl/vmath.h"

enum
{
	PARTICLE_GROUP_SIZE     = 128,
	PARTICLE_GROUP_COUNT    = 8000,
	PARTICLE_COUNT          = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT),
	MAX_ATTRACTORS          = 64
};

class Triangle
{

public:
	Triangle(){}
	~Triangle() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
       init_buffer();
	   init_vertexArray();
	   init_shader();
	}

	/////////////////////////////Render/////////////////////////////
	void Render(float aspect)
	{
		static const GLuint start_ticks = ::GetTickCount() - 100000;
		GLuint current_ticks = ::GetTickCount();
		static GLuint last_ticks = current_ticks;
		float time = ((start_ticks - current_ticks) & 0xFFFFF) / float(0xFFFFF);
		float delta_time = (float)(current_ticks - last_ticks) * 0.075f;

		vmath::vec4 * attractors = (vmath::vec4 *)glMapBufferRange(GL_UNIFORM_BUFFER,
			0,
			32 * sizeof(vmath::vec4),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int i = 0; i < 32; i++)
		{
			attractors[i] = vmath::vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,
				cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f,
				sinf(time * (float)(i + 3) * 5.3f * 20.0f) * cosf(time * (float)(i + 5) * 9.1f) * 100.0f,
				attractor_masses[i]);
		}

		glUnmapBuffer(GL_UNIFORM_BUFFER);

		// If dt is too large, the system could explode, so cap it to
		// some maximum allowed value
		if (delta_time >= 2.0f)
		{
			delta_time = 2.0f;
		}

		// Activate the compute program and bind the position and velocity buffers
		glUseProgram(compute_prog);
		glBindImageTexture(0, velocity_tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(1, position_tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		// Set delta time
		glUniform1f(dt_location, delta_time);
		// Dispatch
		glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		vmath::mat4 mvp = vmath::perspective(45.0f, aspect, 0.1f, 1000.0f) *
			vmath::translate(0.0f, 0.0f, -60.0f) *
			vmath::rotate(time * 1000.0f, vmath::vec3(0.0f, 1.0f, 0.0f));

		// Clear, select the rendering program and draw a full screen quad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(render_prog);
		glUniformMatrix4fv(0, 1, GL_FALSE, mvp);
		glBindVertexArray(render_vao);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		// glPointSize(2.0f);
		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		last_ticks = current_ticks;
	}

	void Shutdown()
	{
		glUseProgram(0);
		glDeleteProgram(compute_prog);
		glDeleteProgram(render_prog);
		glDeleteVertexArrays(1, &render_vao);
	}

private:
	void init_shader();
	void init_buffer();
	void init_vertexArray();

private:
	// Compute program
	GLuint  compute_prog;
	GLint   dt_location;

	// Posisition and velocity buffers
	union
	{
		struct
		{
			GLuint position_buffer;
			GLuint velocity_buffer;
		};
		GLuint buffers[2];
	};

	// TBOs
	union
	{
		struct
		{
			GLuint position_tbo;
			GLuint velocity_tbo;
		};
		GLuint tbos[2];
	};

	// Attractor UBO
	GLuint  attractor_buffer;

	// Program, vao and vbo to render a full screen quad
	GLuint  render_prog;
	GLuint  render_vao;
	GLuint  render_vbo;

	// Mass of the attractors
	float attractor_masses[MAX_ATTRACTORS];

	OGLShader AppShader;
	OGLShader CompShader;
};



static inline float random_float()
{
	float res;
	unsigned int tmp;
	static unsigned int seed = 0xFFFF0C59;

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

void Triangle::init_buffer()
{
	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);

	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(vmath::vec4), NULL, GL_DYNAMIC_COPY);

	vmath::vec4 * positions = (vmath::vec4 *)glMapBufferRange(GL_ARRAY_BUFFER,
		                      0,
		                      PARTICLE_COUNT * sizeof(vmath::vec4),
		                      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i] = vmath::vec4(random_vector(-10.0f, 10.0f), random_float());
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	/////////////////////////////////////////////////////////////////////////////////////

	glBindBuffer(GL_ARRAY_BUFFER, velocity_buffer);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(vmath::vec4), NULL, GL_DYNAMIC_COPY);

	vmath::vec4 * velocities = (vmath::vec4 *)glMapBufferRange(GL_ARRAY_BUFFER,
		0,
		PARTICLE_COUNT * sizeof(vmath::vec4),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		velocities[i] = vmath::vec4(random_vector(-0.1f, 0.1f), 0.0f);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);


	/////////////////////////////////////////////////////////////////////////////
	glGenTextures(2, tbos);

	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_BUFFER, tbos[i]);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, buffers[i]);
	}

	glGenBuffers(1, &attractor_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, attractor_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(vmath::vec4), NULL, GL_STATIC_DRAW);

	for (int i = 0; i < MAX_ATTRACTORS; i++)
	{
		attractor_masses[i] = 0.5f + random_float() * 0.5f;
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, attractor_buffer);
}

void Triangle::init_shader()
{
	AppShader.init();
	AppShader.attach(GL_VERTEX_SHADER, "simple.vert");
	AppShader.attach(GL_FRAGMENT_SHADER, "simple.frag");
	AppShader.link();
	render_prog = AppShader.GetProgram();

	CompShader.init();
	CompShader.attach(GL_COMPUTE_SHADER, "simple.comp");
	CompShader.link();
	compute_prog = CompShader.GetProgram();
	dt_location = glGetUniformLocation(compute_prog, "dt");
}

void Triangle::init_vertexArray()
{

}

#endif


