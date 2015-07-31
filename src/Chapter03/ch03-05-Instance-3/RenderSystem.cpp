#include "RenderSystem.h"
#include "ogl/oglDebug.h"
namespace byhj
{


void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch03-02-IndexDraw";
}

void RenderSystem::v_Init()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_Instance.Init();
}

void RenderSystem::v_Render()
{
	static const glm::vec4 bgColor(0.2f, 0.4f, 0.5f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &one);

	static const GLfloat aspect = GetAspect();

	m_Instance.Render(aspect);
	byhj::OGLDebug de;
	de.Init();
	de.Debug();

}

void RenderSystem::v_Shutdown()
{
	m_Instance.Shutdown();
}


}