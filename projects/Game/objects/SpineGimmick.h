#pragma once
#include "MapChipField.h"

class SpineGimmick
{
public:
	enum class State {
		kActive,
		kInactive,
	};


	void Initialize(MapChipField* mapChipField);

	State GetState() const { return state_; }

	void ChangeState(State state);

private:

	State state_ = State::kActive;

	MapChipField* mapChipField_ = nullptr;
};

