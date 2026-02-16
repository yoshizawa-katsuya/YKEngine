#pragma once

/// <summary>
/// StateからEnemySpawnObjectにアクセスするためのクラス
/// </summary>
class EnemySpawnObjectStateContext
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemySpawnObjectStateContext() = default;

	/// <summary>
	/// ウェーブ開始更新処理。
	/// </summary>
	virtual void UpdateWaveStart() = 0;

	/// <summary>
	/// ウェーブ中間更新処理。
	/// </summary>
	virtual void UpdateWaveInterval() = 0;

	/// <summary>
	/// ウェーブ終了更新処理。
	/// </summary>
	virtual void UpdateWaveEnd() = 0;

	/// <summary>
	/// 開始処理が完了していればtrueを返す。
	/// </summary>
	virtual bool IsWaveStartEnd() const = 0;

	/// <summary>
	/// 中間処理が完了していればtrueを返す。
	/// </summary>
	virtual bool IsWaveIntervalEnd() const = 0;

	/// <summary>
	/// タイマーをリセットする。
	/// </summary>
	virtual void ResetTimer() = 0;

	/// <summary>
	/// タイマーを切り替え時間に初期化する。
	/// </summary>
	virtual void SetTimerToSwitchTime() = 0;

	/// <summary>
	/// 敵出現処理。
	/// </summary>
	virtual void SpawnEnemies() = 0;
};

