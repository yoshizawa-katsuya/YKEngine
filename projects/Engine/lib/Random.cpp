#include "Random.h"

Random* Random::instance_ = nullptr;

Random* Random::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Random();
	}
	return instance_;
}

void Random::Initialize()
{
	//ランダムエンジンの初期化
	std::mt19937 randomEngine(seedGenerator_());
	randomEngine_ = randomEngine;
}

void Random::Finalize()
{
	//インスタンスを破棄
	delete instance_;
	instance_ = nullptr;
}