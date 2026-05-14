#pragma once
#include "Vector3.h"

/// <summary>
/// 各StateからEnemyクラスにアクセスするためのクラス。
/// </summary>
class EnemyStateContext
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyStateContext() = default;

	/// <summary>
	/// 死亡部の情報構造体。
	/// </summary>
	struct DieInfo
	{
		YKEngine::Vector3 bulletVelocity;
		YKEngine::Vector3 bulletPosition;
	};

	/// <summary>
	/// 接近更新。
	/// </summary>
	virtual void UpdateApproach() = 0;

	/// <summary>
	/// メイン更新。
	/// </summary>
	virtual void UpdateMain() = 0;

	/// <summary>
	/// 離脱更新。
	/// </summary>
	virtual void UpdateLeave() = 0;

	/// <summary>
	/// 死亡更新。
	/// </summary>
	virtual void UpdateDead() = 0;

	/// <summary>
	/// メイン部の初期化。
	/// </summary>
	virtual void MainInitialize() = 0;

	/// <summary>
	/// 離脱部の初期化。
	/// </summary>
	virtual void LeaveInitialize() = 0;

	/// <summary>
	/// 死亡部の初期化。
	/// </summary>
	virtual void DeadInitialize() = 0;

	/// <summary>
	/// 離脱ステートに移行するならtrue
	/// </summary>
	virtual bool IsLeave() = 0;

	/// <summary>
	/// 離脱ステートからメインステートに移行するならtrue
	/// </summary>
	virtual bool IsMain() = 0;

	/// <summary>
	/// レールカメラに映っていればtrue
	/// </summary>
	virtual bool IsInRailCamera() = 0;

	/// <summary>
	/// 死亡部の情報を返す。
	/// </summary>
	virtual DieInfo GetDieInfo() = 0;

	/// <summary>
	/// 死亡していればtrueを返す。
	/// </summary>
	virtual bool IsDeadStatus() = 0;
};

