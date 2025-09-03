#pragma once

class IPlayerState
{
public:

	virtual void Enter() = 0;

	virtual void Update() = 0;

	virtual void Exit() = 0;
};

