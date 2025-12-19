#pragma once
#include "AnimatedSprite.h"

namespace YKEngine
{
	class Fade;
}

enum class StagingType
{
	kFade,		//フェード
	kEye,		//目開閉
};

class SceneChangeStaging
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SceneChangeStaging* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

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
	/// シーンの始まり開始
	/// </summary>
	void BeginSceneStart(StagingType stagingType);

	/// <summary>
	/// シーンの終わり開始
	/// </summary>
	void BeginSceneEnd(StagingType stagingType);

	/// <summary>
	/// 演出が終了したか
	/// </summary>
	bool IsFinished();

private:

	// シングルトンインスタンス。リソースリークチェックのため明示的破棄用にポインタで保持。
	static SceneChangeStaging* instance_;

	SceneChangeStaging() = default;
	~SceneChangeStaging() = default;
	SceneChangeStaging(SceneChangeStaging&) = delete;
	const SceneChangeStaging& operator=(SceneChangeStaging&) = delete;

	//シーンの状態
	enum class Status
	{
		kStart,		//開始
		kNone,		//通常
		kEnd,		//終了
	};

	Status status_ = Status::kNone;	//現在のシーン状態

	StagingType stagingType_ = StagingType::kEye;//演出タイプ

	//スプライト
	std::unique_ptr<YKEngine::AnimatedSprite> spriteSceneChange_;//シーンチェンジのスプライト

	std::unique_ptr<YKEngine::Fade> fade_;//フェード
	const float fadeDuration_ = 1.0f;//フェード時間
};
