#include "AppearGimmick.h"

void AppearGimmick::Initialize(MapChipField* mapChipField)
{
	state_ = State::kInactive;

	mapChipField_ = mapChipField;
}

void AppearGimmick::ChangeState(State state)
{
	if (state == State::kInactive)
	{
		uint32_t numCellHorizontal = mapChipField_->GetNumCellHorizontal();

		uint32_t numCellVirtical = mapChipField_->GetNumCellVirtical();

		for (uint32_t y = 0; y < numCellVirtical; y++) {
			for (uint32_t x = 0; x < numCellHorizontal; x++) {
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kActiveAppear) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kInactiveAppear);

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
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kInactiveAppear) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kActiveAppear);
				}
			}
		}
	}

	state_ = state;
}
