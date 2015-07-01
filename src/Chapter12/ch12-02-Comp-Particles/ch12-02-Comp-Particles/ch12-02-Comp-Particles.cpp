#include <gl/glew.h>
#include <glm/glm.hpp>

#include "ogl/oglApp.h"

#include "triangle.h"

class OGLRenderSystem : public byhj::Application
{
public:
	OGLRenderSystem() {}
	~OGLRenderSystem() {}

	void v_InitInfo()
	{
		windowInfo.title += "Triangle";
	}
	void v_Init()
	{
		triangle.Init();
	}
	void v_Render()
	{
		static const glm::vec4 bgColor(0.2f, 0.4f, 0.5f, 1.0f);
		glClearBufferfv(GL_COLOR, 0, &bgColor[0]);

		triangle.Render(GetAspect());
	}
	void v_Shutdown()
	{
		triangle.Shutdown();
	}

private:
	Triangle triangle;
};

CALL_MAIN(OGLRenderSystem);