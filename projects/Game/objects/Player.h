#pragma once
#include "Input.h"
class Camera;
class MapChipField;

#include "BaseCharacter.h"
#include "IPlayerState.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerJumpState.h"

class Player : public BaseCharacter
{
public:

	void Initialize(BaseModel* model) override;

	void Update() override;

	void Draw(Camera* camera) override;

	void ChangeState(std::unique_ptr<IPlayerState> state);

private: // 非公開メンバ関数

	void HandleInput();

	void StateUpdate();

	

private:

	std::unique_ptr<IPlayerState> state_ = nullptr;

};

