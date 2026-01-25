#pragma once

/// <summary>
/// StateからdemoPlayerにアクセスするためのコンテキスト
/// </summary>
class DemoPlayerStateContext
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DemoPlayerStateContext() = default;

	/// <summary>
	/// 開始部の更新
	/// </summary>
	virtual void UpdateStart() = 0;

	/// <summary>
	/// メインの更新
	/// </summary>
	virtual void UpdateMain() = 0;

	/// <summary>
	/// 終了部の更新
	/// </summary>
	virtual void UpdateEnd() = 0;

	/// <summary>
	/// アニメーションが終了したらtrueを返す
	/// </summary>
	virtual bool IsEndAnimation() const = 0;

	/// <summary>
	/// シーンが終わるときtrueを返す
	/// </summary>
	virtual bool IsSceneEnd() const = 0;

	/// <summary>
	/// 開始部の処理が完了した後の処理
	/// </summary>
	virtual void AfterStartComplete() = 0;

	/// <summary>
	/// 終了部の更新処理が始まる直前の処理
	/// </summary>
	virtual void BeforeEnd() = 0;

};

