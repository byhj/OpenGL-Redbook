#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/App.h"
#include "Rectangle.h"

namespace byhj
{


class RenderSystem : public byhj::ogl::App
{
public:
	RenderSystem()  {}
	~RenderSystem() {}


public:
	void v_InitInfo() override;
	void v_Init()     override;
	void v_Render()   override;
	void v_Shutdown() override;
	void v_KeyCallback(GLFWwindow* Triangle, int key, int scancode, int action, int mode) override;

private:

	byhj::Rectangle m_Rectangle;
};


}
#endif