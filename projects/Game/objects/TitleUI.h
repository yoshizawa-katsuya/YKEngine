#pragma once
#include <memory>

namespace YKEngine
{
	class Sprite;
}

class TitleUI
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

	std::unique_ptr<YKEngine::Sprite> spriteLogo_;//タイトルロゴ
	std::unique_ptr<YKEngine::Sprite> spritePressA_;//Aボタンを押してくださいのスプライト

};

