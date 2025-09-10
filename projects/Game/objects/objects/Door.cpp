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
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kClosedDoor) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kOpenedDoor);

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
				if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kOpenedDoor) {

					mapChipField_->SetMapChipTypeByIndex(x, y, MapChipType::kClosedDoor);
				}
			}
		}
	}

	state_ = state;
}
