#pragma once
#include "LeftPlayer.h"
#include "RightPlayer.h"
#include "Laser.h"

class PlayerManager
{
public:

	void Initialize();

	void Update();

	void Draw(YKEngine::Camera* camera);

	const LeftPlayer* GetLeftPlayer() const { return leftPlayer_.get(); }
	const RightPlayer* GetRightPlayer() const { return rightPlayer_.get(); }

private:

	//プレイヤー
	std::unique_ptr<LeftPlayer> leftPlayer_;
	std::unique_ptr<RightPlayer> rightPlayer_;

	//レーザー
	std::unique_ptr<Laser> laser_;

	int32_t hp_ = 5;
};

