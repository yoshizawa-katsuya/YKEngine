#pragma once

/// <summary>
/// StateからClearSceneにアクセスするためのクラス。
/// </summary>
class ClearSceneStateContext
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearSceneStateContext() = default;

	/// <summary>
	/// 終了部の更新。
	/// </summary>
	virtual void UpdateEnd() = 0;

	/// <summary>
	/// シーン遷移演出が終わっていたらtrueを返す。
	/// </summary>
	virtual bool IsEndSceneChangeStaging() const = 0;

	/// <summary>
	/// 終了部が開始する際に行う処理。
	/// </summary>
	virtual void EnterEnd() = 0;
};

