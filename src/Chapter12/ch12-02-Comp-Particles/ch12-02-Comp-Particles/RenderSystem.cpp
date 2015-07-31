#include "RenderSystem.h"
#include <glm/glm.hpp>

namespace byhj
{


void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch01-01-Rectangle";
}

void RenderSystem::v_Init()
{
	m_Rectangle.Init();
}

void RenderSystem::v_Render()
{
	static const glm::vec4 bgColor(0.2f, 0.4f, 0.5f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &bgColor[0]);

	static const GLfloat aspect = GetAspect();

	m_Rectangle.Render(aspect);
}

void RenderSystem::v_Shutdown()
{
	m_Rectangle.Shutdown();
}


}