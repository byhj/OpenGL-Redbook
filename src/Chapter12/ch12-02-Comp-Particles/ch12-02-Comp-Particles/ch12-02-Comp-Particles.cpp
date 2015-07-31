#include "RenderSystem.h"
#include <memory>

int main(int argc, const char **argv)
{
	auto app = std::make_shared<byhj::RenderSystem>();

	app->Run(app);

	return 0;
}