#pragma once
#include <memory>

namespace YKEngine
{
	class Sprite;
}

class ClearUI
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

private:


	/// <summary>
	/// UIの位置を設定
	/// </summary>
	void SetUIPosition();

	std::unique_ptr<YKEngine::Sprite> spriteClear_;//ステージクリアのスプライト
	std::unique_ptr<YKEngine::Sprite> spritePressA_;//Aボタンを押してくださいのスプライト

	float t_ = 0.0f; //点滅の時間計測用変数

	// PressAのスプライトが現れていくときはtrue、消えていくときはfalse
	bool isPressAVisible_ = true;

};
