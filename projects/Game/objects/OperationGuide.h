#pragma once
#include <memory>

namespace YKEngine
{
	class Sprite;
	class Input;
	class GlobalVariables;
}

/// <summary>
/// 操作方法を表示するUIを管理するクラス
/// </summary>
class OperationGuide
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize(bool* isDodge);

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

	/// <summary>
	/// スティックのアイコンを移動させる
	/// </summary>
	void MoveStickIcon();

	bool isDraw_;	//描画するかどうか

	YKEngine::GlobalVariables* globalVariables_;	//グローバル変数のポインタ
	YKEngine::Input* input_;

	std::unique_ptr<YKEngine::Sprite> RTriggerSprite_;	//右トリガーのスプライト
	std::unique_ptr<YKEngine::Sprite> RTriggerReactionSprite_;	//右トリガーの反応のスプライト
	std::unique_ptr<YKEngine::Sprite> LTriggerSprite_;	//左トリガーのスプライト
	std::unique_ptr<YKEngine::Sprite> LTriggerReactionSprite_;	//左トリガーの反応のスプライト
	std::unique_ptr<YKEngine::Sprite> RStickSprite_;	//右スティックのスプライト
	std::unique_ptr<YKEngine::Sprite> LStickSprite_;	//左スティックのスプライト

	bool isPushRTrigger_ = false;	//右トリガーを押しているか
	bool* isDodge_;	//回避状態かどうかを示すフラグへのポインタ
};

