#include <gl/glew.h>
#include <glm/glm.hpp>

#include "ogl/oglApp.h"
#include "Texture.h"

class OGLRenderSystem : public byhj::Application
{
public:
	OGLRenderSystem() {}
	~OGLRenderSystem() {}

	void v_InitInfo()
	{
		windowInfo.title += "Texture";
	}
	void v_Init()
	{
		texture.Init();
	}
	void v_Render()
	{
		static const glm::vec4 bgColor(0.2f, 0.4f, 0.5f, 1.0f);
		glClearBufferfv(GL_COLOR, 0, &bgColor[0]);

		texture.Render();
	}
	void v_Shutdown()
	{
		texture.Shutdown();
	}

private:
	Texture texture;
};

CALL_MAIN(OGLRenderSystem);