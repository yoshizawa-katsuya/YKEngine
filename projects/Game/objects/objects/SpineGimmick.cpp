#include "SpineGimmick.h"

void SpineGimmick::Initialize(MapChipField* mapChipField)
{
	state_ = State::kInactive;

	mapChipField_ = mapChipField;
}

void SpineGimmick::ChangeState(State state)
{
	if (state == State::kInactive)
	{
		uint32_t numCellHorizontal = mapChipField_->GetNumCellHorizontal();

		uint32_t numCellVirtical = mapChipField_->GetNumCellVirtical();

		for (uint32_t y = 0; y < numCellVirtical; y++) {
			for (uint32_t x = 0; x < numCellHorizontal; x++) {
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kActiveSpine) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kInactiveSpine);

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
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kInactiveSpine) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kActiveSpine);
				}
			}
		}
	}

	state_ = state;
}
