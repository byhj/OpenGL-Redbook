#include "oglDebug.h"

#include "glDebug.h"

namespace byhj
{


void OGLDebug::Init()
{

}

void OGLDebug::Debug()
{
	static bool Check = true;

	if (Check)
	{
		CheckDebugLog();
		Check = false;
	}
}

}
