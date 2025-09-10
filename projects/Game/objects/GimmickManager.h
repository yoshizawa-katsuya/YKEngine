#pragma once
#include <memory>

// engine
#include "InstancingObjects.h"
#include "ModelPlatform.h"

// game
#include "Door.h"
#include "SpineGimmick.h"
#include "DisappearGimmick.h"
#include "AppearGimmick.h"

class GimmickManager
{
public:
	enum class GimmickType {
		kDoor,
		kSpine,
		kDisappear,
		kAppear,
	};
	void Initialize(MapChipField* mapChipField);

	void Draw(Camera* camera);

	void DoorGimmickChangeState(Door::State state) {
		doorGimmick_->ChangeState(state);
	}

	void SpineGimmickChangeState(SpineGimmick::State state) {
		spineGimmick_->ChangeState(state);
	}

	void DisappearGimmickChangeState(DisappearGimmick::State state) {
		disappearGimmick_->ChangeState(state);
	}

	void AppearGimmickChangeState(AppearGimmick::State state) {
		appearGimmick_->ChangeState(state);
	}

	Door::State              GetDoorState()      const { return doorGimmick_->GetState(); }
	SpineGimmick::State      GetSpineState()     const { return spineGimmick_->GetState(); }
	DisappearGimmick::State  GetDisappearState() const { return disappearGimmick_->GetState(); }
	AppearGimmick::State     GetAppearState()    const { return appearGimmick_->GetState(); }

	// 토글
	void ToggleDoor() {
		auto cur = doorGimmick_->GetState();
		doorGimmick_->ChangeState(cur == Door::State::kClosed ? Door::State::kOpened : Door::State::kClosed);
	}
	void ToggleSpine() {
		auto cur = spineGimmick_->GetState();
		spineGimmick_->ChangeState(cur == SpineGimmick::State::kActive ? SpineGimmick::State::kInactive : SpineGimmick::State::kActive);
	}
	void ToggleDisappear() {
		auto cur = disappearGimmick_->GetState();
		disappearGimmick_->ChangeState(cur == DisappearGimmick::State::kInactive ? DisappearGimmick::State::kActive : DisappearGimmick::State::kInactive);
	}
	void ToggleAppear() {
		auto cur = appearGimmick_->GetState();
		appearGimmick_->ChangeState(cur == AppearGimmick::State::kInactive ? AppearGimmick::State::kActive : AppearGimmick::State::kInactive);
	}

	InstancingObjects* GetTriggers(GimmickType type);

private:

	std::shared_ptr<BaseModel> modelTrigger_;

	std::shared_ptr<BaseModel> modelBlock_;

	std::shared_ptr<BaseModel> modelSpine_;

	/// 棘
	std::unique_ptr<InstancingObjects> spineTriggers_;

	// 消えたり現れたりするトラップ
	std::unique_ptr<InstancingObjects> gimmickSpines_;

	/// ドア
	std::unique_ptr<InstancingObjects> doorTriggers_;

	std::unique_ptr<InstancingObjects> doors_;

	/// 消えるブロック
	std::unique_ptr<InstancingObjects> disappearTriggers_;

	std::unique_ptr<InstancingObjects> disappearBlocks_;

	/// 現れるブロック
	std::unique_ptr<InstancingObjects> appearTriggers_;
	std::unique_ptr<InstancingObjects> appearBlocks_;

	// ドアギミック
	std::unique_ptr<Door> doorGimmick_;
	std::unique_ptr<SpineGimmick> spineGimmick_;
	std::unique_ptr<DisappearGimmick> disappearGimmick_;
	std::unique_ptr<AppearGimmick> appearGimmick_;
};

