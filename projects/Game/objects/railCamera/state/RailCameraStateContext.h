#pragma once

class RailCameraStateContext
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RailCameraStateContext() = default;

	/// <summary>
	/// メインの更新。
	/// </summary>
	virtual void UpdateMain() = 0;

	/// <summary>
	/// ゲームオーバーの更新。
	/// </summary>
	virtual void UpdateGameOver() = 0;

	/// <summary>
	/// クリアシーンの更新。
	/// </summary>
	virtual void UpdateClearScene() = 0;

	/// <summary>
	/// ゲームオーバーならtrue
	/// </summary>
	virtual bool IsGameOver() const = 0;

	/// <summary>
	/// 補完係数t_をリセット
	/// </summary>
	virtual void ReaetT() = 0;

	/// <summary>
	/// クリアシーンステートに入る際の処理
	/// </summary>
	virtual void EnterClearScene() = 0;
};
