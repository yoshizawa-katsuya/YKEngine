#pragma once
#include <memory>

namespace YKEngine
{
	class Sprite;
}

class OperationGuide
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw();

	/// <summary>
	/// スタートからメインに移行する際の処理。
	/// </summary>
	void ExitStart();

private:

	/// <summary>
	/// UIの位置を設定。
	/// </summary>
	void SetUIPositions();

	bool isDraw_;	//描画するかどうか

	std::unique_ptr<YKEngine::Sprite> RTriggerSprite_;	//右トリガーのスプライト
	std::unique_ptr<YKEngine::Sprite> LTriggerSprite_;	//左トリガーのスプライト
	std::unique_ptr<YKEngine::Sprite> RStickSprite_;	//右スティックのスプライト
	std::unique_ptr<YKEngine::Sprite> LStickSprite_;	//左スティックのスプライト

	std::unique_ptr<YKEngine::Sprite> playerIconSprite_;	//プレイヤーアイコンのスプライト
	std::unique_ptr<YKEngine::Sprite> dodgeIconSprite_;	//回避アイコンのスプライト
	std::unique_ptr<YKEngine::Sprite> shotIconSprite_;	//ショットアイコンのスプライト
	std::unique_ptr<YKEngine::Sprite> reticleIconSprite_;	//照準アイコンのスプライト
};

