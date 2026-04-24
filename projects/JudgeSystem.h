#pragma once
#include "WorldTransform.h"
#include "GameType.h"

enum class JudgeResult {
	None,
	Hit,          // 成功
	SuccessSquat, // しゃがみ成功
	Miss          // ミス
};

class JudgeSystem
{
public:

	// メイン判定
	static JudgeResult Judge(
		const PoseDir& playerState,
		const PoseDir& wallState,
		const YKEngine::WorldTransform& playerTrans,
		const YKEngine::WorldTransform& wallTrans
	);

private:

	// 当たり判定（位置）
	static bool IsHit(
		const YKEngine::WorldTransform& player,
		const YKEngine::WorldTransform& wall
	);

	// 一致判定（ポーズ＋向き）
	static bool IsMatch(
		const PoseDir& p,
		const PoseDir& w
	);
};

