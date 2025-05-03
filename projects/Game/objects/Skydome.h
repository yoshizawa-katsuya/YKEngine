#pragma once
#include "BaseModel.h"
#include "WorldTransform.h"
#include "Rigid3dObject.h"
#include "Camera.h"

class Skydome {
public:

	//初期化
	void Initialize(BaseModel* model);

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Rigid3dObject> object_;

};