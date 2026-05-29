#pragma once
#include "My3dObject.h"
#include "WorldTransform.h"
#include "GameType.h"
#include "LevelDataLoader.h"

class YKEngine::Camera;
class WallModels;

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
	/// <param name="wallModels">壁のモデル</param>
	void Initialize(const YKEngine::WallData& wallData, bool* isStart, YKEngine::WorldTransform* parent, WallModels* wallModels);

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

	bool GetIsDead() const { return isDead_; }

	void SetIsCollision(bool isCollision) { isCollision_ = isCollision; }

private:
	void UpdateColorForDebug();

private:

	//壁のモデル
	WallModels* wallModels_ = nullptr;

	//Transform変数
	YKEngine::WorldTransform worldTransform_;

	//3Dオブジェクト
	std::unique_ptr<YKEngine::My3dObject> object_;

	//壁の状態
	PoseDir state_;

	//フェイント用のポーズ（オプション）
	std::optional<PlayerPose> feintPose_;

	bool* isStart_ = nullptr;

	//壁の移動速度
	float speed_ = 0.1f;

	//自機との衝突判定をとっていればtrue、これからならfalse
	bool isCollision_ = false;

	bool isDead_ = false; //壁がプレイヤーを通り過ぎて消える位置に到達していればtrue、まだならfalse
};