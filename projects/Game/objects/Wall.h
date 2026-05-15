#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "GameType.h"
#include "LevelDataLoader.h"

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
	/// <param name="wallData">壁のデータ</param>
	/// <param name="isStart">流れ始めるかどうかのフラグのポインタ</param>
	/// <param name="parent">親のワールド変換</param>
	void Initialize(const YKEngine::WallData& wallData, bool* isStart, YKEngine::WorldTransform* parent);

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

	void SetIsCollision(bool isCollision) { isCollision_ = isCollision; }

private:
	void UpdateColorForDebug();

private:
	YKEngine::WorldTransform worldTransform_;

	std::unique_ptr<YKEngine::My3dObject> object_;

	//壁の状態
	PoseDir state_;

	bool* isStart_ = nullptr;

	//壁の移動速度
	float speed_ = 0.1f;

	//自機との衝突判定をとっていればtrue、これからならfalse
	bool isCollision_ = false;
};

