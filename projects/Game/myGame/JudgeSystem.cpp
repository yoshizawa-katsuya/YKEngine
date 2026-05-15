#include "JudgeSystem.h"
#include <cmath>

using namespace YKEngine;

bool JudgeSystem::IsHit(const WorldTransform& player,
    const WorldTransform& wall)
{
    // 距離差
    float dx = fabs(player.translation_.x - wall.translation_.x);
    float dz = fabs(player.translation_.z - wall.translation_.z);

    // 判定幅
    const float kHitRabgeX = 0.5f;
    const float kHitRabgeZ = 0.5f;

    // 両方範囲内なら当たり
    return(dx < kHitRabgeX && dz < kHitRabgeZ);
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