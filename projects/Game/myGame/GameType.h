#pragma once

enum class PlayerPose
{
    Base,
    Squat,
    A,
    B,
    C,
    D
};

enum class PlayerDirection
{
    Left = 0,
    Front = 1,
    Right = 2,

	Count   //使用する方向の数
};

// 共通構造体
struct PoseDir
{
    PlayerPose pose;
    PlayerDirection dir;
};