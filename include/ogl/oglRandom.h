#ifndef OGLRANDOM_H
#define OGLRANDOM_H

#include <random>
#include <glm/glm.hpp>

namespace byhj
{

class OGLRandom
{

public:

	float GetFloat(float begin, float end)
	{
		static std::uniform_real_distribution<float> ran(begin, end);

		return ran(eng);
	}

	glm::vec4 GetColor()
	{
		static std::uniform_real_distribution<float> ran(0.0f, 1.0f);
		for (int i = 0; i != 4; ++i)
			rColor[i] = ran(eng);

		return rColor;
	}

private:
	std::default_random_engine eng;
	glm::vec4 rColor;
};


}
#endif