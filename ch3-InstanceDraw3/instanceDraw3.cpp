#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace std;

#include <shader.h>
#include <vbm.cpp>

const int Width = 800, Height = 640; //窗口宽高
const int  INSTANCE_COUNT = 100 ; //绘制实例数

    // Member variables
float aspect;

GLuint color_buffer;
GLuint model_matrix_buffer;
GLuint color_tbo;
GLuint model_matrix_tbo;
GLuint render_prog;
GLuint color_tbo_loc;
GLuint view_matrix_loc, model_matrix_tbo_loc;
GLint projection_matrix_loc;
VBObject object;
Shader instanceShader("instance Shader");

void load_shaders()
{
	instanceShader.init();
	instanceShader.attach(GL_VERTEX_SHADER, "instance.vert");
	instanceShader.attach(GL_FRAGMENT_SHADER, "instance.frag");
	instanceShader.link();
	instanceShader.use();
	render_prog = instanceShader.program;  //获取矩阵的uniform位置
    view_matrix_loc = glGetUniformLocation(render_prog, "view_matrix");
    projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");
	// Set up the TBO samplers，纹理缓存对象
	color_tbo_loc = glGetUniformLocation(render_prog, "color_tbo");
	model_matrix_tbo_loc = glGetUniformLocation(render_prog, "model_matrix_tbo");
	// Set them to the right texture unit indices,，设置纹理单元索引
	glUniform1i(color_tbo_loc, 0);
	glUniform1i(model_matrix_tbo_loc, 1);

}

void init(void)
{

	load_shaders();
	glClearColor(0.0, 0.0, 0.0, 0.0); //背景为黑色
    glEnable(GL_CULL_FACE);  //背面剔除
    glDisable(GL_DEPTH_TEST); //不启用深度测试
	    // Load the object
    object.LoadFromVBM("../media/objects/armadillo_low.vbm", 0, 1, 2);
    // Bind its vertex array object so that we can append the instanced attributes

    glGenTextures(1, &color_tbo); //创建纹理缓存对象
    glBindTexture(GL_TEXTURE_BUFFER, color_tbo);

    glm::vec4 colors[INSTANCE_COUNT]; //实例模型颜色

    for (int n = 0; n < INSTANCE_COUNT; n++) {
        float a = float(n) / 4.0f;
        float b = float(n) / 5.0f;
        float c = float(n) / 6.0f;

        colors[n][0] = 0.5f + 0.25f * (sinf(a + 1.0f) + 1.0f);
        colors[n][1] = 0.5f + 0.25f * (sinf(b + 2.0f) + 1.0f);
        colors[n][2] = 0.5f + 0.25f * (sinf(c + 3.0f) + 1.0f);
        colors[n][3] = 1.0f;
    }

    glGenBuffers(1, &color_buffer);  //将颜色数据传入到纹理缓存对象中
    glBindBuffer(GL_TEXTURE_BUFFER, color_buffer);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, color_buffer);
    // Now we set up the color array. We want each instance of our geometry
    // to assume a different color, so we'll just pack colors into a buffer
    // object and make an instanced vertex attribute out of it.
    glGenTextures(1, &model_matrix_tbo);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, model_matrix_tbo);

    glGenBuffers(1, &model_matrix_buffer);
    glBindBuffer(GL_TEXTURE_BUFFER, model_matrix_buffer);
    glBufferData(GL_TEXTURE_BUFFER, INSTANCE_COUNT * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, model_matrix_buffer);
    glActiveTexture(GL_TEXTURE0);
	    // Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	aspect = float(h) / float(w);

}

void display(void)
{
	    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);
   static float q = 0.0f;
   static const glm::vec3 X(1.0f, 0.0f, 0.0f);
   static const glm::vec3 Y(0.0f, 1.0f, 0.0f);
   static const glm::vec3 Z(0.0f, 0.0f, 1.0f);

 // Set model matrices for each instance
    glm::mat4 matrices[INSTANCE_COUNT];

    for (int n = 0; n < INSTANCE_COUNT; n++) {
        float a = 50.0f * float(n) / 4.0f;
        float b = 50.0f * float(n) / 5.0f;
        float c = 50.0f * float(n) / 6.0f;

        matrices[n] = glm::rotate(glm::mat4(1.0), a + t * 360.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0), b + t * 360.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0), c + t * 360.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
                      glm::translate(glm::mat4(1.0), glm::vec3(10.0f + a, 40.0f + b, 50.0f + c) );
    }

    // Bind the weight VBO and change its data
    glBindBuffer(GL_TEXTURE_BUFFER, model_matrix_buffer);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(matrices), matrices, GL_DYNAMIC_DRAW);

    // Activate instancing program
    glUseProgram(render_prog);

    // Set up the view and projection matrices
    glm::mat4 view_matrix(glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1500.0f))
		          * glm::rotate(glm::mat4(1.0), t * 360.0f * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::mat4 projection_matrix(glm::frustum(-1.0f, 1.0f, -aspect, aspect, 1.0f, 5000.0f));

	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    // Render INSTANCE_COUNT objects
    object.Render(0, INSTANCE_COUNT);

	glutSwapBuffers();
	//glutPostRedisplay();
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);//The display mode is necessary
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(Width, Height);
	//glutInitContextVersion(4, 3);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Triangle");
	GLenum res = glewInit();  //glewInit() use first in order using other function
	if (res != GLEW_OK) 
		cout << "Init the glew Error" << glewGetErrorString(res) << endl;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}


