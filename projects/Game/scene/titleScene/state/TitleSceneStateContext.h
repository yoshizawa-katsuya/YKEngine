#pragma once
class TitleSceneStateContext
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	virtual ~TitleSceneStateContext() = default;

	/// <summary>
	/// 終了部の更新。
	/// </summary>
	virtual void UpdateEnd() = 0;

	/// <summary>
	/// シーン遷移演出が終了していたらtrueを返す。
	/// </summary>
	virtual bool IsSceneStagingEnd() const = 0;

	/// <summary>
	/// シーンが終了するときの遷移演出の開始。
	/// </summary>
	virtual void BeginSceneEndStaging() = 0;
};

