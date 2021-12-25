#include <random>
#include "Tools.h"

std::random_device rd;
std::mt19937 gen(rd());

int Random(int min, int max)
{
	std::uniform_int_distribution<> distr(min, max);
	return distr(gen);
}

float Random(float min, float max)
{
	std::uniform_real_distribution<float> distr(min, max);
	return distr(gen);
}
