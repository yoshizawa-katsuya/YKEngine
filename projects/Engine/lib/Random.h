#pragma once
#include <random>
#include "Vector3.h"

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

	/// <summary>
	/// float型の乱数を(min, max)の範囲で取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>乱数</returns>
	float GetFloat(float min, float max);

	/// <summary>
	/// Vector3型の乱数を(min, max)の範囲で取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>乱数</returns>
	Vector3 GetVector3(const Vector3& min, const Vector3& max);

	/// <summary>
	/// Vector3型の乱数を(min, max)の範囲で取得
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>乱数</returns>
	Vector3 GetVector3(float min, float max);

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