#include "RenderSystem.h"

namespace byhj
{


void RenderSystem::v_InitInfo()
{
	windowInfo.title += "ch01-01-Triangle";
}

void RenderSystem::v_Init()
{
	m_Triangle.Init();
}

void RenderSystem::v_Render()
{

	m_Triangle.Render();
}

void RenderSystem::v_Shutdown()
{
	m_Triangle.Shutdown();
}


}