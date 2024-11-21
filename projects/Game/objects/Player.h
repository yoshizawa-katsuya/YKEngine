#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Animation.h"
class Camera;
class MapChipField;

class Player
{
public:

	void Initialize(BaseModel* model);

	void Update();

	void Draw(Camera* camera);

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const;

private:

	//Transform変数を作る
	WorldTransform worldTransform_;

	std::unique_ptr<Base3dObject> object_;

private:
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) const;
};

