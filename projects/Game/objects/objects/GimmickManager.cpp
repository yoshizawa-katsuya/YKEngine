#include "GimmickManager.h"

void GimmickManager::Initialize(MapChipField* mapChipField)
{
	doorGimmick_ = std::make_unique<Door>();
	doorGimmick_->Initialize(mapChipField);

	spineGimmick_ = std::make_unique<SpineGimmick>();
	spineGimmick_->Initialize(mapChipField);

	disappearGimmick_ = std::make_unique<DisappearGimmick>();
	disappearGimmick_->Initialize(mapChipField);

	appearGimmick_ = std::make_unique<AppearGimmick>();
	appearGimmick_->Initialize(mapChipField);

	ModelPlatform* modelPlatform = ModelPlatform::GetInstance();

	modelTrigger_ = modelPlatform->CreateRigidModel("./resources/GimmickR", "GimmickR.obj");

	modelBlock_ = modelPlatform->CreateRigidModel("./resources/block", "block.obj");

	modelSpine_ = modelPlatform->CreateRigidModel("./resources/spine", "spine.obj");

	doors_ = std::make_unique<InstancingObjects>();
	doors_->Initialize(modelBlock_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal());
	doors_->PreUpdate();

	doorTriggers_ = std::make_unique<InstancingObjects>();
	doorTriggers_->Initialize(modelTrigger_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal()); // トリガー用のモデルに変更
	doorTriggers_->PreUpdate();

	spineTriggers_ = std::make_unique<InstancingObjects>();
	spineTriggers_->Initialize(modelTrigger_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal()); // トリガー用のモデルに変更
	spineTriggers_->PreUpdate();

	gimmickSpines_ = std::make_unique<InstancingObjects>();
	gimmickSpines_->Initialize(modelSpine_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal());
	gimmickSpines_->PreUpdate();

	disappearBlocks_ = std::make_unique<InstancingObjects>();
	disappearBlocks_->Initialize(modelBlock_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal());
	disappearBlocks_->PreUpdate();

	disappearTriggers_ = std::make_unique<InstancingObjects>();
	disappearTriggers_->Initialize(modelTrigger_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal()); // トリガー用のモデルに変更
	disappearTriggers_->PreUpdate();

	appearBlocks_ = std::make_unique<InstancingObjects>();
	appearBlocks_->Initialize(modelBlock_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal());
	appearBlocks_->PreUpdate();

	appearTriggers_ = std::make_unique<InstancingObjects>();
	appearTriggers_->Initialize(modelTrigger_.get(), mapChipField->GetNumCellVirtical() * mapChipField->GetNumCellHorizontal()); // トリガー用のモデルに変更
	appearTriggers_->PreUpdate();

	WorldTransform worldTransform = {};

	std::function<void(uint32_t, uint32_t)> setWorldTransform = [&worldTransform, mapChipField](uint32_t x, uint32_t y) {
		worldTransform.Initialize();
		worldTransform.translation_ = mapChipField->GetMapChipPositionByIndex(x, y);
		worldTransform.UpdateMatrix();
		};

	for (uint32_t y = 0; y < mapChipField->GetNumCellVirtical(); y++) {
		for (uint32_t x = 0; x < mapChipField->GetNumCellHorizontal(); x++) {
			MapChipType mapChipType = mapChipField->GetMapChipTypeByIndex(x, y);
			switch (mapChipType)
			{

			case MapChipType::kDoorTrigger:

				setWorldTransform(x, y);
				doorTriggers_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kClosedDoor:

				setWorldTransform(x, y);
				doors_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kInactiveSpine:

				setWorldTransform(x, y);
				gimmickSpines_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kSpineTrigger:

				setWorldTransform(x, y);
				spineTriggers_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kInactiveDisappear:

				setWorldTransform(x, y);
				disappearBlocks_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kDisappearTrigger:

				setWorldTransform(x, y);
				disappearTriggers_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kInactiveAppear:

				setWorldTransform(x, y);
				appearBlocks_->WorldTransformUpdate(worldTransform);

				break;

			case MapChipType::kAppearTrigger:

				setWorldTransform(x, y);
				appearTriggers_->WorldTransformUpdate(worldTransform);

				break;

			default:
				break;
			}
		}
	}
}
void GimmickManager::Draw(Camera* camera)
{
	//door
	doorTriggers_->CameraUpdate(camera);
	doorTriggers_->Draw();

	if (doorGimmick_->GetState() == Door::State::kClosed) {
		doors_->CameraUpdate(camera);
		doors_->Draw();
	}

	spineTriggers_->CameraUpdate(camera);
	spineTriggers_->Draw();

	if (spineGimmick_->GetState() == SpineGimmick::State::kActive) {
		gimmickSpines_->CameraUpdate(camera);
		gimmickSpines_->Draw();
	}

	disappearTriggers_->CameraUpdate(camera);
	disappearTriggers_->Draw();

	if (disappearGimmick_->GetState() == DisappearGimmick::State::kInactive) {
		disappearBlocks_->CameraUpdate(camera);
		disappearBlocks_->Draw();
	}
	
	appearTriggers_->CameraUpdate(camera);
	appearTriggers_->Draw();

	if (appearGimmick_->GetState() == AppearGimmick::State::kActive) {
		appearBlocks_->CameraUpdate(camera);
		appearBlocks_->Draw();
	}

}

InstancingObjects* GimmickManager::GetTriggers(GimmickType type)
{
	switch (type) {
	case GimmickType::kDoor:

		return doorTriggers_.get();

		break;

	case GimmickType::kSpine:

		return spineTriggers_.get();

		break;

	case GimmickType::kDisappear:
		
		return disappearTriggers_.get();

		break;

	case GimmickType::kAppear:

		return appearTriggers_.get();

		break;

	default:

		return nullptr;
		break;
	}
}
