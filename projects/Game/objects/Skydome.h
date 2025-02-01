#pragma once
#include "RigidModel.h"
#include "Rigid3dObject.h"
#include "WorldTransform.h"
#include "Camera.h"

///< summary>
/// 天球
///</summary>
class Skydome {
public:
	/// 初期化
	void Initialize(RigidModel* model);

	// 更新
	void Update();

	// 描画
	void Draw(Camera* camera);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	std::unique_ptr<Rigid3dObject> object_;
};