#pragma once

#include "MapChipField.h"

class Door
{
public:
	enum class State {
		kClosed,
		kOpenning,
		kOpened,
		kClosing,
	};


	void Initialize(MapChipField* mapChipField);

	State GetState() const { return state_; }

	void ChangeState(State state);

private:

	State state_ = State::kClosed;

	float timer_ = 0.0f;

	const float kOpenCloseTime_ = 1.0f; // 開閉にかかる時間

	MapChipField* mapChipField_ = nullptr;
};

