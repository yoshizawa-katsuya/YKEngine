#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "GameType.h"

class YKEngine::Camera;

/// <summary>
/// 壁クラス
/// </summary>
class Wall
{
public:
	void Initialize(const YKEngine::Vector3& translate);

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

