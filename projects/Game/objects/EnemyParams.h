#pragma once
#include "bullet/EnemyBulletType.h"

//弾を撃つ敵のパラメータ
struct ShotEnemyParams
{
	EnemyBulletType bulletType;
	float fireInterval; //弾の発射間隔。秒
	float bulletSpeed;  //弾の速度
};