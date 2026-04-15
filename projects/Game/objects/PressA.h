#pragma once
#include <memory>
#include "Vector2.h"

namespace YKEngine
{
	class Sprite;
}

/// <summary>
/// Aボタンを押してくださいのUIを管理するクラス
/// </summary>
class PressA
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 位置を設定
	/// </summary>
	/// <param name="position">設定する位置</param>
	void SetPosition(YKEngine::Vector2 position);

private:

	std::unique_ptr<YKEngine::Sprite> spritePressA_;//Aボタンを押してくださいのスプライト

	float t_ = 0.0f; //点滅の時間計測用変数

	// PressAのスプライトが現れていくときはtrue、消えていくときはfalse
	bool isPressAVisible_ = true;
};

