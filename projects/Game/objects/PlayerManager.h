#pragma once
#include "LeftPlayer.h"
#include "RightPlayer.h"

class PlayerManager
{
public:

	void Initialize();

	void Update();

	void Draw(YKEngine::Camera* camera);

private:

	//プレイヤー
	std::unique_ptr<LeftPlayer> leftPlayer_;
	std::unique_ptr<RightPlayer> rightPlayer_;
};

