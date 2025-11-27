#include "Random.h"

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
