#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>

using namespace std;

#include <shader.h>
#include <vbm.cpp>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define FRUSTUM_DEPTH       800.0f
#define DEPTH_TEXTURE_SIZE  2048

float aspect;
const int Width = 800, Height = 640; //窗口宽高
struct
{
   GLint model_view_projection_matrix;
} render_light_uniforms;

    // FBO to render depth with
GLuint  depth_fbo;
GLuint  depth_texture;
GLuint render_scene_prog;
GLuint render_light_prog;

  struct
{
    GLint model_matrix;
    GLint view_matrix;
    GLint projection_matrix;
    GLint shadow_matrix;
    GLint light_position;
    GLint material_ambient;
    GLint material_diffuse;
    GLint material_specular;
    GLint material_specular_power;
} render_scene_uniforms;

// Ground plane
GLuint  ground_vbo;
GLuint  ground_vao;
VBObject object;
GLint current_width;
GLint current_height;

#define INSTANCE_COUNT 100

Shader lightShader("shadow Shader");
Shader sceneShader("scene Shader");

void load_shaders()
{
	lightShader.init();
	lightShader.attach(GL_VERTEX_SHADER, "shadowmap_shadow.vert");
	lightShader.attach(GL_FRAGMENT_SHADER, "shadowmap_shadow.frag");
	lightShader.link();
	lightShader.use();
	render_light_prog = lightShader.program;  //获取矩阵的uniform位置
    // Get the location of the projetion_matrix uniform
    render_light_uniforms.model_view_projection_matrix = glGetUniformLocation(render_light_prog, "model_view_projection_matrix");

    sceneShader.init();
	sceneShader.attach(GL_VERTEX_SHADER, "shadowmap_scene.vert");
	sceneShader.attach(GL_FRAGMENT_SHADER, "shadowmap_scene.frag");
	sceneShader.link();
	sceneShader.use();
	render_scene_prog  = sceneShader.program;
    render_scene_uniforms.model_matrix = glGetUniformLocation(render_scene_prog, "model_matrix");
    render_scene_uniforms.view_matrix = glGetUniformLocation(render_scene_prog, "view_matrix");
    render_scene_uniforms.projection_matrix = glGetUniformLocation(render_scene_prog, "projection_matrix");
    render_scene_uniforms.shadow_matrix = glGetUniformLocation(render_scene_prog, "shadow_matrix");
    render_scene_uniforms.light_position = glGetUniformLocation(render_scene_prog, "light_position");
    render_scene_uniforms.material_ambient = glGetUniformLocation(render_scene_prog, "material_ambient");
    render_scene_uniforms.material_diffuse = glGetUniformLocation(render_scene_prog, "material_diffuse");
    render_scene_uniforms.material_specular = glGetUniformLocation(render_scene_prog, "material_specular");
    render_scene_uniforms.material_specular_power = glGetUniformLocation(render_scene_prog, "material_specular_power");
	// Set the depth texture uniform to unit 0
    glUseProgram(render_scene_prog);
    glUniform1i(glGetUniformLocation(render_scene_prog, "depth_texture"), 0);
}

void init(void)
{
	load_shaders();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

    // Create FBO to render depth into
    glGenFramebuffers(1, &depth_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_texture, 0);
    glDrawBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Upload geometry for the ground plane
    static const float ground_vertices[] = {
        -500.0f, -50.0f, -500.0f, 1.0f,
        -500.0f, -50.0f,  500.0f, 1.0f,
         500.0f, -50.0f,  500.0f, 1.0f,
         500.0f, -50.0f, -500.0f, 1.0f,
    };

    static const float ground_normals[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &ground_vao);
    glGenBuffers(1, &ground_vbo);
    glBindVertexArray(ground_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices) + sizeof(ground_normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(ground_normals), ground_normals);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(ground_vertices));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // Load the object
    object.LoadFromVBM("armadillo_low.vbm", 0, 1, 2);
}

void reshape(int w, int h)
{
	current_width = w;
	current_height = h;
	aspect = float(w) / float(h);
}

void DrawScene(bool depth_only);

void display(void)
{
    float t = float(GetTickCount() & 0xFFFF) / float(0xFFFF);
    static float q = 0.0f;
    static const glm::vec3 X(1.0f, 0.0f, 0.0f);
    static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
    static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

    glm::vec3 light_position = glm::vec3(sinf(t * 6.0f * 3.141592f) * 300.0f, 200.0f, cosf(t * 4.0f * 3.141592f) * 100.0f + 250.0f);

    // Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Matrices for rendering the scene
    glm::mat4 scene_model_matrix = glm::rotate(glm::mat4(1.0), t * 720.0f, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 scene_view_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -300.0f));
    glm::mat4 scene_projection_matrix = glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, FRUSTUM_DEPTH);
    const glm::mat4 scale_bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
                                        glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
                                        glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                                        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

    // Matrices used when rendering from the light's position
	glm::mat4 light_view_matrix = glm::lookAt(light_position, glm::vec3(0.0f),glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 light_projection_matrix(glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, FRUSTUM_DEPTH));

    // Now we render from the light's position into the depth buffer.
    // Select the appropriate program
    glUseProgram(render_light_prog);
	glUniformMatrix4fv(render_light_uniforms.model_view_projection_matrix, 1, GL_FALSE, glm::value_ptr(light_projection_matrix * light_view_matrix * scene_model_matrix));

    // Bind the 'depth only' FBO and set the viewport to the size of the depth texture
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
    glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);

    // Clear
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable polygon offset to resolve depth-fighting isuses
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    // Draw from the light's point of view
    DrawScene(true);
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Restore the default framebuffer and field of view
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, current_width, current_height);

    // Now render from the viewer's position
    glUseProgram(render_scene_prog);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Setup all the matrices
	glUniformMatrix4fv(render_scene_uniforms.model_matrix, 1, GL_FALSE, glm::value_ptr(scene_model_matrix));
    glUniformMatrix4fv(render_scene_uniforms.view_matrix, 1, GL_FALSE, glm::value_ptr(scene_view_matrix));
    glUniformMatrix4fv(render_scene_uniforms.projection_matrix, 1, GL_FALSE, glm::value_ptr(scene_projection_matrix));
    glUniformMatrix4fv(render_scene_uniforms.shadow_matrix, 1, GL_FALSE, glm::value_ptr(scale_bias_matrix * light_projection_matrix * light_view_matrix));
    glUniform3fv(render_scene_uniforms.light_position, 1, glm::value_ptr(light_position));

    // Bind the depth texture
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Draw
    DrawScene(false);
}

void DrawScene(bool depth_only)
{
    // Set material properties for the object
    if (!depth_only)  {
        glUniform3fv(render_scene_uniforms.material_ambient, 1, glm::value_ptr(glm::vec3(0.1f, 0.0f, 0.2f)));
        glUniform3fv(render_scene_uniforms.material_diffuse, 1, glm::value_ptr(glm::vec3(0.3f, 0.2f, 0.8f)));
        glUniform3fv(render_scene_uniforms.material_specular, 1,glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform1f(render_scene_uniforms.material_specular_power, 25.0f);
    }

    // Draw the object
    object.Render();

    // Set material properties for the ground
    if (!depth_only)  {
        glUniform3fv(render_scene_uniforms.material_ambient, 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
        glUniform3fv(render_scene_uniforms.material_diffuse, 1, glm::value_ptr( glm::vec3(0.1f, 0.5f, 0.1f)));
        glUniform3fv(render_scene_uniforms.material_specular, 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
        glUniform1f(render_scene_uniforms.material_specular_power, 3.0f);
    }

    // Draw the ground
    glBindVertexArray(ground_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}


int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	////glutInitContextVersion(4, 3);
	////glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("shadow map");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}


