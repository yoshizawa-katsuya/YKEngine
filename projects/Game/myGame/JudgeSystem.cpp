#include "JudgeSystem.h"
#include <cmath>

using namespace YKEngine;

bool JudgeSystem::IsHit(const WorldTransform& player,
    const WorldTransform& wall)
{
    // Z距離で判定（レーンゲーム前提）
    float dz = fabs(player.translation_.z - wall.translation_.z);

    return dz < 0.5f; // 調整ポイント
}

bool JudgeSystem::IsMatch(const PoseDir& p,
    const PoseDir& w)
{
    return (p.pose == w.pose &&
        p.dir == w.dir);
}

JudgeResult JudgeSystem::Judge(
    const PoseDir& playerState,
    const PoseDir& wallState,
    const WorldTransform& playerTrans,
    const WorldTransform& wallTrans)
{
    // 当たってない 
    if (!IsHit(playerTrans, wallTrans))
        return JudgeResult::None;

    // ===== しゃがみ判定 =====
    if (playerState.pose == PlayerPose::Squat)
    {
        // 壁もしゃがみなら成功（方向無視）
        if (wallState.pose == PlayerPose::Squat)
        {
            return JudgeResult::SuccessSquat;
        }
        else
        {
            return JudgeResult::Miss;
        }
    }

    // ===== 通常判定 =====
    if (IsMatch(playerState, wallState))
    {
        return JudgeResult::Hit;
    }

    return JudgeResult::Miss;

}