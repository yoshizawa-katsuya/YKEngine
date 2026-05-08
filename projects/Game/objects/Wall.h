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

	//自機との判定ラインに到達していればtrue、まだならfalse
	bool GetIsLineJudged() const { return worldTransform_.translation_.z <= 0.0f; }

	const YKEngine::WorldTransform& GetWorldTransform()const { return worldTransform_; }

	PoseDir GetState() const { return state_; }

	bool GetIsCollision() const { return isCollision_; }

	void SetIsStart(bool isStart) { isStart_ = isStart; }

	void SetIsCollision(bool isCollision) { isCollision_ = isCollision; }

private:
	void UpdateColorForDebug();

private:
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

	PoseDir state_;

	bool isStart_ = false;

	//自機との衝突判定をとっていればtrue、これからならfalse
	bool isCollision_ = false;
};

