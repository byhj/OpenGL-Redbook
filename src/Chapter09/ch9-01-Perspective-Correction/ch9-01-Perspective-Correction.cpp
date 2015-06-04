#include <GL/glew.h>
#include <sb6/shader.h>
#include <sb6/sb6.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PerspectiveApp: public sb6::Application
{
public:
	PerspectiveApp():program(0), use_perspective(true)
	{
		memset(keys, 0, sizeof(keys));
	}
	void vInit();
	void vRender();

	void init_shader();
	void init_texture();

	void keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		//Enter Key 'p' to change Perspective correction
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS && key == GLFW_KEY_P)
				use_perspective= !use_perspective;
		}
	}

private:
	 GLuint perspective_loc, mvp_matrix_loc, tex_loc;
     GLuint program, tex;
     bool use_perspective;
     Shader perspectiveShader;
	 bool keys[1024];
};

DECLARE_MAIN(PerspectiveApp);

void PerspectiveApp::init_shader()
{
	perspectiveShader.attach(GL_VERTEX_SHADER, "perspective.vert");
	perspectiveShader.attach(GL_FRAGMENT_SHADER, "perspective.frag");
	perspectiveShader.link();
	perspectiveShader.interfaceInfo();
	perspectiveShader.use();
	program = perspectiveShader.GetProgram();

	perspective_loc = glGetUniformLocation(program, "use_perspective"); 
	mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
	tex_loc = glGetUniformLocation(program, "tex_checker");
	glUniform1i(tex_loc, 0);
}

#define BWBW 0x00, 0xFF, 0x00, 0xFF
#define WBWB 0xFF, 0x00, 0xFF, 0x00

const static unsigned char checker_data[] = {
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW,
	WBWB, WBWB,
	BWBW, BWBW
};
void PerspectiveApp::init_texture()
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 8, 8); //LEVEL must set >=1

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RED, GL_UNSIGNED_BYTE, checker_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void PerspectiveApp::vInit()
{
	init_shader();
	init_texture();
}

void PerspectiveApp::vRender()
{
	static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
	static const GLfloat ones[] = { 1.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, ones);

	static float currentTime ;
	currentTime = glfwGetTime();
    glUseProgram(program);

	//set the MVP matrix
	float time = (float)currentTime * (float)3.14159 * 0.1f;
	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.5f))
		                * glm::rotate(glm::mat4(1.0), currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 proj_matrix = glm::perspective(45.0f, 720.0f / 640.0f, 0.1f, 1000.0f);
	glm::mat4 mvp_matrix = proj_matrix * mv_matrix ; 
	glUniformMatrix4fv(mvp_matrix_loc, 1, 0, &mvp_matrix[0][0]);

	//We use PerspectiveApp correction default
	glUniform1i(perspective_loc, use_perspective);

	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}




