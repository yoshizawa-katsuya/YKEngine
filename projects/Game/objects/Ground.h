#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

///< summary>
/// 地面
///</summary>
class Ground {
public:

	/// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	// モデル
	Model* model_ = nullptr;

};

