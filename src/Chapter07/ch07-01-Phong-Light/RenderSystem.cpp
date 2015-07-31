#include "RenderSystem.h"

namespace byhj
{


void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch01-01-Torus";
}

void RenderSystem::v_Init()
{
	m_Torus.Init();
}

void RenderSystem::v_Render()
{
	static const glm::vec4 bgColor(0.2f, 0.4f, 0.5f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);
	static const GLfloat one = 1.0f;
	glClearBufferfv(GL_DEPTH, 0, &one);

	static const GLfloat aspect = GetAspect();
	m_Torus.Render(aspect);
}

void RenderSystem::v_Shutdown()
{
	m_Torus.Shutdown();
}


}