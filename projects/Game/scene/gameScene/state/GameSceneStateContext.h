#pragma once

/// <summary>
/// StateからGameSceneにアクセスするためのコンテキスト
/// </summary>
class GameSceneStateContext
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameSceneStateContext() = default;

	/// <summary>
	/// 開始部の更新。
	/// </summary>
	virtual void UpdateStart() = 0;

	/// <summary>
	/// メイン部の更新。
	/// </summary>
	virtual void UpdateMain() = 0;

	/// <summary>
	/// クリア部の更新。
	/// </summary>
	virtual void UpdateGameClear() = 0;

	/// <summary>
	/// ゲームオーバー部の更新。
	/// </summary>
	virtual void UpdateGameOver() = 0;

	/// <summary>
	/// タイトルへ戻る部の更新。
	/// </summary>
	virtual void UpdateTitleReturn() = 0;

	/// <summary>
	/// スタート処理が終わったらtrue
	/// </summary>
	/// <returns> スタート処理が終わったらtrue </returns>
	virtual bool StartCompleted() = 0;

	/// <summary>
	/// クリアしていたらtrueを返す。
	/// </summary>
	virtual bool CheckGameClear() = 0;

	/// <summary>
	/// ゲームオーバーしていたらtrueを返す。
	/// </summary>
	virtual bool CheckGameOver() = 0;

	/// <summary>
	/// タイトルへ戻るときにtrueを返す。
	/// </summary>
	virtual bool IsReturnToTitle() = 0;

	/// <summary>
	/// ゲームオーバーに移行する際の処理。
	/// </summary>
	virtual void ProcessGameOver() = 0;

	/// <summary>
	/// クリアに移行する際の処理。
	/// </summary>
	virtual void ProcessGameClear() = 0;

	/// <summary>
	/// シーン終了演出の開始。
	/// </summary>
	virtual void StartSceneEndStaging() = 0;
};

