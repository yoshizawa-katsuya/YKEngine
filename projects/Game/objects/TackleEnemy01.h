#pragma once
#include "BaseEnemy.h"

/// <summary>
/// プレイヤーに向かって突進する敵キャラ。
/// 突進中はプレイヤーを追尾する。
/// </summary>
class TackleEnemy01 : public BaseEnemy
{
public:

	~TackleEnemy01() override;

	void OnCollision([[maybe_unused]] Collider* other) override;

private:

	void UpdateApproach() override;
	void UpdateMain() override;
	void UpdateLeave() override;

	void Move() override;

	void SetColliderID() override;

	bool isHoming_ = true;

};

