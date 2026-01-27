#pragma once
#include <memory>
#include <unordered_map>
#include "Vector2.h"

namespace YKEngine
{
	class Sprite;
	class SceneManager;
}

class Pause
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

	// ポーズ中かどうかを設定
	void SetIsPause(bool isPause);

	// ポーズ中かどうかを取得
	bool GetIsPause() const { return isPause_; }

	// タイトルに戻るが選択されているかを取得
	bool GetIsReturnToTitle() const { return isReturnToTitle_; }

private:

	/// <summary>
	/// UIの位置を設定
	/// </summary>
	void SetUIPositions();

	// UIの名前
	enum class UIName
	{
		kReturnToTitle,
		kContinue,

		kCount,
	};

	//スプライト
	std::unique_ptr<YKEngine::Sprite> backgroundSprite_;
	std::unique_ptr<YKEngine::Sprite> cursorSprite_;

	//カーソルの目標位置
	int32_t cursorTarget_ = static_cast<int32_t>(UIName::kContinue);

	//UIのスプライトリスト
	std::unordered_map<UIName, std::unique_ptr<YKEngine::Sprite>> UISprites_;

	//UIの位置リスト
	std::unordered_map<UIName, YKEngine::Vector2> UIPositions_;

	//ポーズ中ならtrue
	bool isPause_ = false;

	//タイトルに戻るが選択されているならtrue
	bool isReturnToTitle_ = false;

	YKEngine::SceneManager* sceneManager_ = nullptr;
};