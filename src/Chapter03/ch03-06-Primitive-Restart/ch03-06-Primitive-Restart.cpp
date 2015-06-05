#include<gl/glew.h>
#include<gl/freeglut.h>
#define GLM_FORCE_RADIANS

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <common/shader.h>


GLuint program;
GLuint modelLoc;
GLuint mvpLoc;
GLuint EBO, VBO, VAO;

#define USE_PRIMITIVE_RESTART//开启图元中断

static const GLfloat positions[] = //顶点数据
    {
        -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 1.0f,
         1.0f,  1.0f, -1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    //颜色数据
static const GLfloat colors[] =
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    };

    // Indices for the triangle strips
static const GLushort indices[] =
    {
        0, 1, 2, 3, 6, 7, 4, 5,         // First strip
        0xFFFF,                         // 重启动标志
        2, 6, 0, 4, 1, 5, 3, 7          // Second strip
    };

Shader cubeShader("triangle shader");  //shader对象
const int Width = 800, Height = 640; //窗口宽高

void load_shaders()
{
	cubeShader.init();
	cubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	cubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	cubeShader.link();
	cubeShader.use();
	program  = cubeShader.GetProgram();
	modelLoc= glGetUniformLocation(program, "model_matrix");
	mvpLoc = glGetUniformLocation(program, "mvp_matrix");
}

void init(void)
{
	load_shaders();
	glClearColor(0.0, 0.0, 0.0, 0.0); 
    glEnable(GL_CULL_FACE);  
    glDisable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);  
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);  
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), NULL, GL_STATIC_DRAW);//先分配内存，不传入数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);
  

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(positions));
    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // mvp矩阵
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -5.0));
	model *= glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//set the view and projection matrix

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 1.0f, 1000.0f);
	glm::mat4 mvp  = projection * view;

    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//使用图元中断，条件编译
#ifdef USE_PRIMITIVE_RESTART   
    // When primitive restart is on, we can call one draw command
    glEnable(GL_PRIMITIVE_RESTART);  //开启图元中断
    glPrimitiveRestartIndex(0xFFFF); //设置中断标志
    glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL); //绘制函数
#else
    //不使用图元中断
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, (const GLvoid *)(9 * sizeof(GLushort)));
#endif
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	//Enter key 'm' to change the display mode
	switch(key) {
	    case 'm': 
		{  
		   static GLenum mode = GL_FILL;
		   mode = ( mode == GL_FILL ? GL_LINE : GL_FILL); 
		   glPolygonMode(GL_FRONT_AND_BACK, mode); 
        }
	    break;
	}
	glutPostRedisplay();
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
		std::cout << "Init the glew Error" << glewGetErrorString(res) << std::endl;
	
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}


