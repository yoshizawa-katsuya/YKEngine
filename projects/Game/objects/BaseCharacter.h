#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"

class BaseCharacter : public Collider{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models_">モデルデータ配列</param>
	virtual void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection);
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw();

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	
	//中心座標を取得
	virtual Vector3 GetCenterPosition() const override;

protected:
	
	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;
};
