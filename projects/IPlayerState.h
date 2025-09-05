#pragma once

class BaseCharacter;
class Player;

class IPlayerState
{
public: // 公開メンバ関数

	virtual void Enter(BaseCharacter* character) = 0;

	virtual void Update() = 0;

	virtual void Exit() = 0;

protected: // 

	Player* player = nullptr;
};

