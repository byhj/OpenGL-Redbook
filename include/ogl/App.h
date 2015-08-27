#ifndef APP_H
#define APP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

#ifdef _WIN32
const int ScreenWidth = static_cast<int>( GetSystemMetrics(SM_CXSCREEN) * 0.75 );
const int ScreenHeight = static_cast<int>(  GetSystemMetrics(SM_CYSCREEN) * 0.75 );
const int PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth)  ;
const int PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight) ;
#else
const int ScreenWidth = 1200;
const int ScreenHeight = 1000;
const int PosX = 300;
const int PosY = 100;
#endif

namespace byhj {

namespace ogl
{


	class App 
	{
	public:
		App() {}
		virtual ~App() {}

	public:
		void Run(std::shared_ptr<App> the_app);

		//Override
		virtual void v_InitInfo() = 0;
		virtual void v_Init()	  = 0;
		virtual void v_Render()	  = 0;
		virtual void v_Shutdown() = 0;

		virtual void v_KeyCallback(GLFWwindow* Triangle, int key, int scancode, int action, int mode) 
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(Triangle, GL_TRUE);
			if (key ==  GLFW_KEY_C && action == GLFW_PRESS)
				glfwSetInputMode(Triangle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		virtual void v_Movement(GLFWwindow *Triangle) {}
		virtual void v_MouseCallback(GLFWwindow* Triangle, double xpos, double ypos) {}
		virtual void v_ScrollCallback(GLFWwindow* Triangle, double xoffset, double yoffset) {}

	protected:
		struct WindowInfo
		{
			WindowInfo():title("OpenGL Redbook:"), 
				        Width(ScreenWidth), 
						Height(ScreenHeight),
				        posX(PosX), 
						posY(PosY) {}

			std::string title;
			int Width;
			int Height;
			int posX, posY;
		}windowInfo;

		float GetAspect();
		int GetScreenWidth();
		int GetScreenHeight();

	protected:
	    static  std::shared_ptr<App> app;

	    static void glfw_key(GLFWwindow * Triangle, int key, int scancode, int action, int mode) 
	    {
	    	app->v_KeyCallback(Triangle,  key,  scancode, action,  mode);
	    }
	    static void glfw_mouse(GLFWwindow* Triangle, double xpos, double ypos)
	    {
	    	app->v_MouseCallback(Triangle,  xpos, ypos);
	    }
	    static void glfw_scroll(GLFWwindow* Triangle, double xoffset, double yoffset)
	    {
	    	app->v_ScrollCallback(Triangle,  xoffset, yoffset);
	    }
	    
	};  //class
}

}  //namespace 



#endif  //