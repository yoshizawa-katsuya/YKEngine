#pragma once

/// <summary>
/// StateからPlayerへ干渉するためのコンテキスト
/// </summary>
class PlayerStateContext
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerStateContext() = default;

	/// <summary>
	/// 開始部の更新
	/// </summary>
	virtual void UpdateStart() = 0;

	/// <summary>
	/// メインの更新
	/// </summary>
	virtual void UpdateMain() = 0;

	/// <summary>
	/// ゲームオーバー時の更新
	/// </summary>
	virtual void UpdateGameOver() = 0;

	/// <summary>
	/// ゲームクリア時の更新
	/// </summary>
	virtual void UpdateGameClear() = 0;

	//スタート処理が終わっていたらtrue
	virtual bool StartCompleted() = 0;

	/// <summary>
	/// 開始部の処理が完了した後の処理
	/// </summary>
	virtual void AfterStartComplete() = 0;

	/// <summary>
	/// クリアしていたらtrue
	/// </summary>
	/// <returns>クリアしていたらtrue</returns>
	virtual bool GetIsGameClear() = 0;

	/// <summary>
	/// 死亡していたらtrue
	/// </summary>
	/// <returns>死亡していたらtrue</returns>
	virtual bool IsDead() = 0;

	/// <summary>
	/// ゲームオーバーになった瞬間の回転。
	/// </summary>
	virtual void GameOverRotate() = 0;
};

