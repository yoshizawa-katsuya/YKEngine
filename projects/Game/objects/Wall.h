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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="translate">初期位置</param>
	void Initialize(const YKEngine::Vector3& translate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(YKEngine::Camera* camera);

	const YKEngine::WorldTransform& GetWorldTransform()const { return worldTransform_; }

	PoseDir GetState() const { return state_; }

	void SetIsStart(bool isStart) { isStart_ = isStart; }

private:
	void UpdateColorForDebug();

private:
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

	PoseDir state_;

	bool isStart_ = false;
};

