#pragma once
#include "RigidModel.h"
#include "Rigid3dObject.h"
#include "WorldTransform.h"
#include "Collider.h"
#include "Camera.h"

class BaseCharacter : public Collider{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="objects_">モデルデータ配列</param>
	virtual void Initialize(const std::vector<RigidModel*>& models, RigidModel* colliderModel);
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(Camera* camera);

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	
	//中心座標を取得
	virtual Vector3 GetCenterPosition() const override;

protected:
	
	//モデルデータ配列
	std::vector<std::unique_ptr<Rigid3dObject>> objects_;
	//ワールド変換データ
	WorldTransform worldTransform_;

};
