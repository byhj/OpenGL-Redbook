#include "Debug.h"

#include "glDebug.h"

namespace byhj
{

namespace ogl
{

void Debug::init()
{

}

void Debug::debug()
{
	static bool Check = true;

	if (Check)
	{
		CheckDebugLog();
		Check = false;
	}
}

}

}
