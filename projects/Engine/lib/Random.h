#pragma once
#include <random>

namespace YKEngine
{

class Random
{
public:

	/// <summary>
	/// シングルトンインスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Random* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	const std::mt19937& GetRandomEngine() const { return randomEngine_; }
	std::mt19937* GetRandomEnginePtr() { return &randomEngine_; }

private:

	Random() = default;
	~Random() = default;
	Random(Random&) = default;
	Random& operator=(Random&) = default;

	//ランダムエンジン
	std::random_device seedGenerator_;
	std::mt19937 randomEngine_;
};

}	// namespace YKEngine