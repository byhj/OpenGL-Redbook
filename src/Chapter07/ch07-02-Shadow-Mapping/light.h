#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <GLFW//glfw3.h>

#include "ogl/oglShader.h"
#include "ogl/loadTexture.h"
#include "ogl/vmath.h"
#include "ogl/vbm.h"

using namespace vmath;

#define FRUSTUM_DEPTH       800.0f
#define DEPTH_TEXTURE_SIZE  1024

class Light
{

public:
	Light() {}
	~Light() {}

public:

	/////////////////////////////Init/////////////////////////////
	void Init()
	{
       init_buffer();
	   init_vertexArray();
	   init_shader();
	   init_texture();
	   init_fbo();
	}

	/////////////////////////////Render/////////////////////////////
	void Render(float current_width, float current_height, float aspect)
	{
		float time = glfwGetTime();

		float t = float(GetTickCount() & 0xFFFF) / float(0xFFFF);
		static float q = 0.0f;
		static const vec3 X(1.0f, 0.0f, 0.0f);
		static const vec3 Y(0.0f, 1.0f, 0.0f);
		static const vec3 Z(0.0f, 0.0f, 1.0f);
		//CheckDebugLog();  
		//We change the light pos every frame
		vec3 light_position = vec3(sinf(t * 6.0f * 3.141592f) * 300.0f, 200.0f, cosf(t * 4.0f * 3.141592f) * 100.0f + 250.0f);
		// Setup
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Matrices for rendering the scene
		mat4 scene_model_matrix = rotate(t * 720.0f, Y);
		mat4 scene_view_matrix = translate(0.0f, 0.0f, -300.0f);
		mat4 scene_projection_matrix = frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, FRUSTUM_DEPTH);
		const mat4 scale_bias_matrix = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
			vec4(0.0f, 0.5f, 0.0f, 0.0f),
			vec4(0.0f, 0.0f, 0.5f, 0.0f),
			vec4(0.5f, 0.5f, 0.5f, 1.0f)
			);

		// Matrices used when rendering from the light's position
		mat4 light_view_matrix = lookat(light_position, vec3(0.0f), Y);
		mat4 light_projection_matrix(frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, FRUSTUM_DEPTH));

		// Now we render from the light's position into the depth buffer.
		// Select the appropriate program
		glUseProgram(shadow_prog);
		glUniformMatrix4fv(shadow_mvp_loc, 1, GL_FALSE, light_projection_matrix * light_view_matrix * scene_model_matrix);

		//Bind the framebuffer data to dpeth texture
		glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
		glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);

		// Clear
		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Enable polygon offset to resolve depth-fighting isuses
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.0f, 4.0f);

		// Draw from the light's point of view, we draw the depth to fbo
		DrawScene(true);

		glDisable(GL_POLYGON_OFFSET_FILL);


		// Restore the default framebuffer and field of view
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, current_width, current_height);
		// Now render from the viewer's position

		glUseProgram(base_prog);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Setup all the matrices
		glUniformMatrix4fv(base_uniform_loc.model, 1, GL_FALSE, scene_model_matrix);
		glUniformMatrix4fv(base_uniform_loc.view, 1, GL_FALSE, scene_view_matrix);
		glUniformMatrix4fv(base_uniform_loc.proj, 1, GL_FALSE, scene_projection_matrix);
		glUniformMatrix4fv(base_uniform_loc.shadow_matrix, 1, GL_FALSE, scale_bias_matrix * light_projection_matrix * light_view_matrix);
		glUniform3fv(      base_uniform_loc.lightPos, 1, light_position);

		// Bind the depth texture
		glBindTexture(GL_TEXTURE_2D, depth_texture);
		glGenerateMipmap(GL_TEXTURE_2D);

		DrawScene(false);

	}

	void Shutdown()
	{
		glUseProgram(0);
		glDeleteProgram(shadow_prog);
		glDeleteProgram(base_prog);
		glDeleteBuffers(1, &ground_vbo);
		glDeleteVertexArrays(1, &ground_vao);
	}

private:

	void init_buffer();
	void init_vertexArray();
	void init_shader();
	void init_texture();
	void init_fbo();
	void DrawScene(bool depth_only);
private:




	GLint current_width;
	GLint current_height;
	VBObject g_object;

	OGLShader ObjectShader;
    OGLShader ShadowShader;
	GLuint ground_vao, ground_vbo;
	GLuint base_prog;

	// Member variables
	float aspect;
	// Program to render from the light's position
	GLuint shadow_prog;
	GLuint shadow_mvp_loc;
	GLuint depth_fbo, depth_texture;


	struct 
	{
		GLuint model;
		GLuint view;
		GLuint proj;
		GLuint shadow_matrix;
		GLuint lightPos;
		GLuint mat_ambient;
		GLuint mat_diffuse;
		GLuint mat_specular;
		GLuint mat_specular_power;
	}base_uniform_loc;



};

void Light::DrawScene(bool depth_only)
{
	// Set material properties for the object
	if (!depth_only)
	{
		glUniform3fv(base_uniform_loc.mat_ambient, 1, vec3(0.1f, 0.0f, 0.2f));
		glUniform3fv(base_uniform_loc.mat_diffuse, 1, vec3(0.3f, 0.2f, 0.8f));
		glUniform3fv(base_uniform_loc.mat_specular, 1, vec3(1.0f, 1.0f, 1.0f));
		glUniform1f( base_uniform_loc.mat_specular_power, 25.0f);
	}

	// Draw the object
	g_object.Render();

	// Set material properties for the ground
	if (!depth_only)
	{
		glUniform3fv(base_uniform_loc.mat_ambient, 1, vec3(0.1f, 0.1f, 0.1f));
		glUniform3fv(base_uniform_loc.mat_diffuse, 1, vec3(0.1f, 0.5f, 0.1f));
		glUniform3fv(base_uniform_loc.mat_specular, 1, vec3(0.1f, 0.1f, 0.1f));
		glUniform1f( base_uniform_loc.mat_specular_power, 3.0f);
	}

	// Draw the ground
	glBindVertexArray(ground_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}


// Upload geometry for the ground plane
static const float ground_vertices[] =
{
	-500.0f, -50.0f, -500.0f, 1.0f,
	-500.0f, -50.0f,  500.0f, 1.0f,
	500.0f, -50.0f,  500.0f, 1.0f,
	500.0f, -50.0f, -500.0f, 1.0f,
};

static const float ground_normals[] =
{
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

void Light::init_buffer()
{
	//set the ground data
	glGenBuffers(1, &ground_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices) + sizeof(ground_normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(ground_normals), ground_normals);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	g_object.LoadFromVBM("../../../media/objects/armadillo_low.vbm", 0, 1, 2);
}

void Light::init_vertexArray()
{	
	glGenVertexArrays(1, &ground_vao);
	glBindVertexArray(ground_vao);

	glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(ground_vertices));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Light::init_shader()
{

	ShadowShader.init();
	ShadowShader.attach(GL_VERTEX_SHADER, "shadow.vert");
	ShadowShader.attach(GL_FRAGMENT_SHADER, "shadow.frag");
	ShadowShader.link();
	ShadowShader.interfaceInfo();
	shadow_prog = ShadowShader.GetProgram();
	shadow_mvp_loc = glGetUniformLocation(shadow_prog, "mvp");


	ObjectShader.init();
	ObjectShader.attach(GL_VERTEX_SHADER, "base.vert");
	ObjectShader.attach(GL_FRAGMENT_SHADER, "base.frag");
	ObjectShader.link();
	ObjectShader.interfaceInfo();
	base_prog = ObjectShader.GetProgram();
	base_uniform_loc.model              = glGetUniformLocation(base_prog, "model");
	base_uniform_loc.view               = glGetUniformLocation(base_prog, "view");
	base_uniform_loc.proj               = glGetUniformLocation(base_prog, "proj");
	base_uniform_loc.shadow_matrix      = glGetUniformLocation(base_prog, "shadow_matrix");
	base_uniform_loc.lightPos           = glGetUniformLocation(base_prog, "lightPos");
	base_uniform_loc.mat_ambient        = glGetUniformLocation(base_prog, "mat_ambient");
	base_uniform_loc.mat_diffuse        = glGetUniformLocation(base_prog, "mat_diffuse");
	base_uniform_loc.mat_specular       = glGetUniformLocation(base_prog, "mat_specular");
	base_uniform_loc.mat_specular_power = glGetUniformLocation(base_prog, "mat_specular_power");

	//set the depth texture to unit 0 
	glUseProgram(base_prog);
	glUniform1i(glGetUniformLocation(base_prog, "depth_texture"), 0);
}

void Light::init_texture()
{
	// Create a depth texture
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Light::init_fbo()
{
	//Create a Fbo, render the depth texture
	glGenFramebuffers(1, &depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	//We don't need the color buffer
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif