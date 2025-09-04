#pragma once
#include "IPlayerState.h"

class BaseCharacter;

class PlayerGroundState : public IPlayerState
{
public: // 公開メンバ関数

	void Enter(BaseCharacter* character) override;

	void Update() override;

	void Exit() override;
};

