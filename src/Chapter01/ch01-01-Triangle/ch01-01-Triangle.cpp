#include <gl/glew.h>
#include <glm/glm.hpp>

#include "ogl/oglApp.h"
#include "ogl/oglFont.h"

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
		m_font.Init(GetWidth(), GetHeight());
	}
	void v_Render()
	{

		static const glm::vec4 bgColor(0.2f, 0.4f, 0.5f, 1.0f);
		glClearBufferfv(GL_COLOR, 0, &bgColor[0]);

		triangle.Render();
		std::string fpsStr = "FPS:";
		fpsStr += std::to_string(fps);
		m_font.Render(fpsStr, 10.0f, GetHeight()-25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

	}
	void v_Shutdown()
	{
		triangle.Shutdown();
	}

private:
	OGLFont  m_font;
	Triangle triangle;
};

CALL_MAIN(OGLRenderSystem);