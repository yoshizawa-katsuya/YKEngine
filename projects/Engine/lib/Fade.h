#pragma once
#include "Sprite.h"

/// <summary>
/// フェードクラス。
/// 画面のフェードイン・フェードアウトを管理する。
/// </summary>
class Fade {
public:

	// フェードの状態
	enum class Status {
		None,    // フェードなし
		FadeIn,  // フェードイン中
		FadeOut, // フェードアウト中
	};

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Fade();

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
	/// フェード開始。
	/// </summary>
	/// <param name="status">フェードの状。</param>
	/// <param name="duration">フェードの持続時間</param>
	void Start(Status status, float duration);

	/// <summary>
	/// フェード停止。
	/// </summary>
	void Stop();

	/// <summary>
	/// フェードが終了したかどうか。
	/// </summary>
	/// <returns>終了していればtrue、そうでなければfalse。</returns>
	bool IsFinished();

private:

	std::unique_ptr<Sprite> sprite_; //フェード用スプライト

	//現在のフェードの状態
	Status status_ = Status::None;

	//フェードの持続時間
	float duration_ = 0.0f;
	//経過時間カウンター
	float counter_ = 0.0f;

};
