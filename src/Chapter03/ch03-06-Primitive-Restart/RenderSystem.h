#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ogl/oglApp.h"
#include "cube.h"

namespace byhj
{


class RenderSystem : public byhj::Application
{
public:
	RenderSystem()  {}
	~RenderSystem() {}


public:
	void v_InitInfo() override;
	void v_Init()     override;
	void v_Render()   override;
	void v_Shutdown() override;

private:

	byhj::Cube m_Cube;
};


}
#endif