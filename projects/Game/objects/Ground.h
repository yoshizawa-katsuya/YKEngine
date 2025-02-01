#pragma once
#include "RigidModel.h"
#include "Rigid3dObject.h"
#include "WorldTransform.h"
#include "Camera.h"

///< summary>
/// 地面
///</summary>
class Ground {
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

