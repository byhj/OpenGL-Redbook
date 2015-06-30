#include <gl/glew.h>
#include <glm/glm.hpp>

#include "ogl/oglApp.h"
#include "instance.h"

class OGLRenderSystem : public byhj::Application
{
public:
	OGLRenderSystem() {}
	~OGLRenderSystem() {}

	void v_InitInfo()
	{
		windowInfo.title += "Instance";
	}

	void v_Init()
	{
		instance.Init();
	}

	void v_Render()
	{
		static const glm::vec4 bgColor(0.2f, 0.4f, 0.5f, 1.0f);
		static const float depth = 1.0f;
		glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
		glClearBufferfv(GL_DEPTH, 0, &depth);

		instance.Render( GetAspect() );
	}

	void v_Shutdown()
	{
		instance.Shutdown();
	}

private:
	Instance instance;
};

CALL_MAIN(OGLRenderSystem);