#pragma once
#include "AnimatedSprite.h"
#include "Fade.h"

enum class StagingType
{
	kFade,		//フェード
	kMixFade,	//ミックスフェード
	kEye,		//目開閉
};

/// <summary>
/// シーンチェンジの演出を管理するクラス。
/// </summary>
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
	/// シーンの終わり開始
	/// </summary>
	void BeginSceneEnd(StagingType stagingType, const YKEngine::Vector4& color);

	/// <summary>
	/// 演出が終了したか
	/// </summary>
	bool IsFinished();

	//コンストラクタに渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class SceneChangeStaging;
	};

	//PassKeyを受け取るコンストラクタ
	explicit SceneChangeStaging(ConstructorKey key) {}

private:

	// シングルトンインスタンス
	static std::unique_ptr<SceneChangeStaging> instance_;
	friend struct std::default_delete<SceneChangeStaging>;

	~SceneChangeStaging() = default;
	SceneChangeStaging(SceneChangeStaging&) = delete;
	const SceneChangeStaging& operator=(SceneChangeStaging&) = delete;

	//演出中ならtrue
	bool isStaging_ = false;

	StagingType stagingType_ = StagingType::kEye;//演出タイプ

	//スプライト
	std::unique_ptr<YKEngine::AnimatedSprite> spriteSceneChange_;//シーンチェンジのスプライト

	std::unique_ptr<YKEngine::Fade> fade_;//フェード
	std::unique_ptr<YKEngine::Fade> fadeGray_;//灰色フェード
	const float fadeDuration_ = 1.0f;//フェード時間
};
