#include "DisappearGimmick.h"

void DisappearGimmick::Initialize(MapChipField* mapChipField)
{
	state_ = State::kInactive;

	mapChipField_ = mapChipField;
}

void DisappearGimmick::ChangeState(State state)
{
	if (state == State::kInactive)
	{
		uint32_t numCellHorizontal = mapChipField_->GetNumCellHorizontal();

		uint32_t numCellVirtical = mapChipField_->GetNumCellVirtical();

		for (uint32_t y = 0; y < numCellVirtical; y++) {
			for (uint32_t x = 0; x < numCellHorizontal; x++) {
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kActiveDisappear) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kInactiveDisappear);

				}
			}
		}
	}
	else if (state == State::kActive)
	{
		uint32_t numCellHorizontal = mapChipField_->GetNumCellHorizontal();
		uint32_t numCellVirtical = mapChipField_->GetNumCellVirtical();
		for (uint32_t y = 0; y < numCellVirtical; y++) {
			for (uint32_t x = 0; x < numCellHorizontal; x++) {
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kInactiveDisappear) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kActiveDisappear);
				}
			}
		}
	}

	state_ = state;
}
