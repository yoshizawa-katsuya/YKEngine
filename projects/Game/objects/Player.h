#pragma once
#include "Input.h"
#include "Audio.h"
class Camera;
class MapChipField;
class Input;

#include "BaseCharacter.h"
#include "IPlayerState.h"
#include "ElectricRange.h"

class Player : public BaseCharacter
{
private:

	enum Corner {
		kRightBottom,	//右下
		kLeftBottom,	//左下
		kRightTop,		//右上
		kLeftTop,		//左上

		kNumCorner		//要素数

	};

public:
	/// <summary>
	/// 初期化
	///  </summary>
	void Initialize(BaseModel* model,  const Vector3& position) override;

	/// <summary>
	/// 更新
	///  </summary>
	void Update() override;

	/// <summary>
	/// 描画
	///  </summary>
	void Draw(Camera* camera) override;

	void ChangeState(std::unique_ptr<IPlayerState> state);

	void SetJumpSE(LoopSoundData* jumpSE) { jumpSE_ = jumpSE; }

	bool HitGoal() { return hitGoal_; }

private: // 非公開メンバ関数

	void StateUpdate();

	//どのマップチップに接触しているか
	void CheckHitMapChipType(MapChipType mapChipType) override;

	//移動入力
	void Move();
	
	void SpineCollision(const CollisionMapInfo& info) override;

public:
	ElectricRange* GetElectricRange() const { return electricRange_.get(); }
	//void SetElectricModel(BaseModel* electricModel);
private:

	Input* input_;

	std::unique_ptr<IPlayerState> state_ = nullptr;

	bool hitGoal_ = false;
	//電気範囲
	std::unique_ptr<ElectricRange> electricRange_;

	LoopSoundData* jumpSE_ = nullptr;
};

