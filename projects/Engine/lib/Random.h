#pragma once
#include <random>

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

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	const std::mt19937& GetRandomEngine() const { return randomEngine_; }
	std::mt19937* GetRandomEnginePtr() { return &randomEngine_; }

private:

	// シングルトンインスタンス
	static Random* instance_;

	Random() = default;
	~Random() = default;
	Random(Random&) = default;
	Random& operator=(Random&) = default;

	//ランダムエンジン
	std::random_device seedGenerator_;
	std::mt19937 randomEngine_;
};

