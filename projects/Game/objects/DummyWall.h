#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "GameType.h"

class YKEngine::Camera;
/// <summary>
/// ダミーの壁クラス
/// </summary>
class DummyWall
{
public:
	void Initialize(YKEngine::BaseModel* model);

	void Update();

	void Draw(YKEngine::Camera* camera);

	const YKEngine::WorldTransform& GetWorldTransform()const { return worldTransform_; }

	PoseDir GetState()const { return state_; }

private:
	void UpdateColorForDebug();

private:
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

	PoseDir state_;

	bool isStart_ = false;
};

