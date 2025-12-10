#include "Random.h"

using namespace YKEngine;

Random* Random::GetInstance()
{
	static Random instance;
	return &instance;
}

void Random::Initialize()
{
	//ランダムエンジンの初期化
	std::mt19937 randomEngine(seedGenerator_());
	randomEngine_ = randomEngine;
}

float YKEngine::Random::GetFloat(float min, float max)
{
	if (min > max)
	{
		GetFloat(max, min);
	}
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(randomEngine_);
}

Vector3 YKEngine::Random::GetVector3(const Vector3& min, const Vector3& max)
{
	return Vector3(
		GetFloat(min.x, max.x),
		GetFloat(min.y, max.y),
		GetFloat(min.z, max.z)
	);
}

Vector3 YKEngine::Random::GetVector3(float min, float max)
{
	return Vector3(
		GetFloat(min, max),
		GetFloat(min, max),
		GetFloat(min, max)
	);
}
