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

