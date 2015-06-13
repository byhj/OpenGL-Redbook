#ifndef STEPAPP_H
#define STEPAPP_H

#include "glDebug.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>

#ifdef WIN32
//If PLATFORM IS WIN32, we put the render window to the middle of window
const int g_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
const int g_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
const GLfloat g_Aspect = float(g_ScreenWidth) / float(g_ScreenHeight);
const int g_PosX = (GetSystemMetrics(SM_CXSCREEN) - g_ScreenWidth) / 2;
const int g_PosY = (GetSystemMetrics(SM_CYSCREEN) - g_ScreenHeight) / 2;

#else
const int g_ScreenWidth = 1000;
const int g_ScreenHeight = 800;
const int g_PosX = 300;
const int g_PosY = 100;
#endif

namespace byhj
{
	class Application
	{
	public:
		Application():WindowTitle("OpenGL-Step") {};
		virtual ~Application() {};

	public:


		virtual void v_Init() = 0;
		virtual void v_Render() = 0;
		virtual void v_Shutdown() = 0;
		virtual void reshape(int w, int h)
		{
			glViewport(0, 0, w, h);
		}
		virtual void v_Keyboard(unsigned char key, int x, int y)
		{
			switch(key)
			{
			case 27: // Escape key
				exit (0);
				break;
			}
		}
		virtual void v_PassiveMouse(int xpos, int ypos){ };
		virtual void v_MouseWheel(int wheel, int direction, int x, int y) { };

		void run(byhj::Application *the_app, int argc, char **argv)
		{
			//Notice: It is important send the app handler to the static pointer
			app = the_app;

			glutInit(&argc, argv); 
			glutInitDisplayMode(GLUT_DOUBLE);
			glutInitWindowSize(g_ScreenWidth, g_ScreenHeight);
			glutInitWindowPosition(g_PosX, g_PosY);
			glutCreateWindow(WindowTitle.c_str());

			GLenum res = glewInit();

			if (res != GLEW_OK)
			{
				std::cerr << "Error:" << glewGetErrorString(res) << std::endl;
				return;
			}
			//glEnable(GL_DEBUG_OUTPUT)​;

			const GLubyte *renderer = glGetString( GL_RENDERER );  
			const GLubyte *vendor = glGetString( GL_VENDOR );  
			const GLubyte *version = glGetString( GL_VERSION );  
			const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );  
			GLint major, minor;  
			glGetIntegerv(GL_MAJOR_VERSION, &major);  
			glGetIntegerv(GL_MINOR_VERSION, &minor);  
			std::cout << "GL Vendor    :" << vendor << std::endl;  
			std::cout << "GL Renderer  : " << renderer << std::endl;  
			std::cout << "GL Version (std::string)  : " << version << std::endl;  
			std::cout << "GL Version (integer) : " << major << "." << minor << std::endl;  
			std::cout << "GLSL Version : " << glslVersion << std::endl;    
			std::cout << "------------------------------------------------------------------------------" << std::endl;
			//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			v_Init();
		    //CheckDebugLog();

			glutDisplayFunc(glut_render);
			glutKeyboardFunc(glut_keyboard);
			glutPassiveMotionFunc(glut_passiveMouse);
			glutMouseWheelFunc(glut_mouseWheel);
			glutReshapeFunc(glut_reshape);
			glutMainLoop();
			v_Shutdown();

			}
	protected:
		static byhj::Application *app;
		static void glut_render()
		{
			app->v_Render();
		}
		static void glut_reshape(int w, int h)
		{
			app->reshape(w, h);
		}
		static void glut_keyboard(unsigned char key, int x, int y)
		{
			app->v_Keyboard(key, x, y);
		}
		static void glut_passiveMouse(int xpos, int ypos)
		{
			app->v_PassiveMouse(xpos, ypos);
		}
		static void glut_mouseWheel(int wheel, int direction, int x, int y)
		{
			app->v_MouseWheel(wheel, direction, x, y);
		}
		float GetAspect()
		{
			return (float)g_ScreenWidth / g_ScreenHeight;
		}
		int GetScreenWidth()
		{
			return g_ScreenWidth;
		}
		int GetScreenHeight()
		{
			return g_ScreenHeight;
		}
    private:
		std::string WindowTitle;
	};
}


byhj::Application * byhj::Application::app; 

#define CALL_MAIN(a)                                \
	int main(int argc, char **argv)                 \
{                                                   \
	a *app = new a;                                 \
	app->run(app, argc, argv);                      \
	delete app;                                     \
	return 0;                                       \
}

#endif