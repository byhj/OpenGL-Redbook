#include<gl/glew.h>
#include<gl/freeglut.h>
#include <iostream>

using namespace std;

#include <shader.h>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];

const GLuint  NumVertices = 6;

Shader triangleShader("triangle shader");  //shader对象
const int Width = 800, Height = 640; //窗口宽高

void load_shaders()
{
	triangleShader.init();
	triangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	triangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	triangleShader.link();
	triangleShader.use();
}

/* 浮点格式
struct VertexData {
        GLfloat color[3];
        GLfloat position[4];
};

VertexData vertices[NumVertices] = {
        {{  1.00, 0.00, 0.00 }, { -0.90, -0.90 }},  // Triangle 1
        {{  0.00, 1.00, 0.00 }, {  0.85, -0.90 }},
        {{  0.00, 0.00, 1.00 }, { -0.90,  0.85 }},
        {{  0.04, 0.04, 0.04 }, {  0.90, -0.85 }},  // Triangle 2
        {{  0.40, 0.40, 0.40 }, {  0.90,  0.90 }},
        {{  1.00, 1.00, 1.00 }, { -0.85,  0.90 }}
};
*/

void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	load_shaders();
    // Set up the vertex attributes to vao
    glGenVertexArrays( NumVAOs, VAOs );
    glBindVertexArray( VAOs[Triangles] );

    struct VertexData {
        GLubyte color[4];
        GLfloat position[4];
    };

	    VertexData vertices[NumVertices] = {
        {{ 255,   0,   0, 255 }, { -0.90f, -0.90f }},  // Triangle 1
        {{   0, 255,   0, 255 }, {  0.85f, -0.90f }},
        {{   0,   0, 255, 255 }, { -0.90f,  0.85f }},
        {{  10,  10,  10, 255 }, {  0.90f, -0.85f }},  // Triangle 2
        {{ 100, 100, 100, 255 }, {  0.90f,  0.90f }},
        {{ 255, 255, 255, 255 }, { -0.85f,  0.90f }}
    };

    glGenBuffers( NumBuffers, Buffers );
    glBindBuffer( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices),
                  vertices, GL_STATIC_DRAW );
	glVertexAttribPointer( vColor, 4, GL_UNSIGNED_BYTE,
                           GL_TRUE, sizeof(VertexData), NULL );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT,
                           GL_FALSE, sizeof(VertexData),
						   (GLvoid*)(sizeof(vertices[0].color)) );

    glEnableVertexAttribArray( vColor );
    glEnableVertexAttribArray( vPosition );
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray( VAOs[Triangles] ); //使用当前数据绘制两个三角形
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch(key) {
	    case 'm': {
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);//The display mode is necessary
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
    glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}


