#pragma once
#include "Rigid3dObject.h"

class Discharge {
public:

	//初期化
	void Initialize(BaseModel* model);

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

	void SetPosition(const Vector3& position);

	float GetRadius() const { return worldTransform_.scale_.x; }
	const Vector3& GetPosition() const { return worldTransform_.translation_; }

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	EulerTransform uvTransform_{};

	std::unique_ptr<Rigid3dObject> object_;
};

