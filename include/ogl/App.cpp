#include "App.h"

namespace byhj
{

namespace ogl
{

std::shared_ptr<App> App::app;

void App::Run(std::shared_ptr<App> the_app)
{	
    app = the_app;

	std::cout << "Starting GLFW context" << std::endl;
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}

	v_InitInfo();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow *Triangle = glfwCreateWindow(windowInfo.Width, windowInfo.Height,
		                                  windowInfo.title.c_str(), nullptr, nullptr);
	glfwSetWindowPos(Triangle, windowInfo.posX, windowInfo.posY);
	glfwMakeContextCurrent(Triangle);

	//Key and Mouse callback function
	glfwSetKeyCallback(Triangle, glfw_key);
	glfwSetCursorPosCallback(Triangle, glfw_mouse);
	glfwSetScrollCallback(Triangle, glfw_scroll);

	// GLFW Options
	//glfwSetInputMode(Triangle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (Triangle == NULL)
	{
		std::cerr << "Failed to create GLFW Triangle" << std::endl;
		glfwTerminate();
		return ;
	}	
	glewExperimental = GL_TRUE;

	//Check the GLSL and OpenGL status 
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return ;
	}
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
	std::cout << "--------------------------------------------------------------------------------" 
		      << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); //opengl 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	v_Init();

	glViewport(0, 0, windowInfo.Width, windowInfo.Height);

	while (!glfwWindowShouldClose(Triangle)) 
	{
		glfwPollEvents();
		v_Movement(Triangle);

		//Render for the object
		v_Render();

		glfwSwapBuffers(Triangle);
	}
	v_Shutdown();

	glfwTerminate();
}

float App::GetAspect()
{
	return static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);
}

int App::GetScreenWidth()
{
	return ScreenWidth;
}

int App::GetScreenHeight()
{
	return ScreenHeight;
}

}

}
