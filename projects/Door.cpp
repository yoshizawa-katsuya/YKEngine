#include "Door.h"

void Door::Initialize(MapChipField* mapChipField)
{
	state_ = State::kClosed;

	mapChipField_ = mapChipField;
}

void Door::ChangeState(State state)
{
	if (state == State::kOpened)
	{
		uint32_t numCellHorizontal = mapChipField_->GetNumCellHorizontal();

		uint32_t numCellVirtical = mapChipField_->GetNumCellVirtical();

		for (uint32_t y = 0; y < numCellVirtical; y++) {
			for (uint32_t x = 0; x < numCellHorizontal; x++) {
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kDoor) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kTransparentBlock);

				}
			}
		}
	}
	else if (state == State::kClosed)
	{
		uint32_t numCellHorizontal = mapChipField_->GetNumCellHorizontal();
		uint32_t numCellVirtical = mapChipField_->GetNumCellVirtical();
		for (uint32_t y = 0; y < numCellVirtical; y++) {
			for (uint32_t x = 0; x < numCellHorizontal; x++) {
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kTransparentBlock) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kDoor);
				}
			}
		}
	}

	state_ = state;
}
