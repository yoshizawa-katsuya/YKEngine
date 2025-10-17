#pragma once
#include "Collider.h"
#include "Rigid3dObject.h"

/// <summary>
/// キャラクターの基底クラス。
/// 継承して使用する。
/// キャラクターはコライダーを持つためColliderクラスを継承する。
/// </summary>
class BaseCharacter : public Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(BaseModel* model);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(Camera* camera);

	bool IsDead() const { return isDead_; }

protected:

	std::unique_ptr<Rigid3dObject> object_;
	//ワールド変換データ
	WorldTransform characterWorldTransform_;

	//デスフラグ
	bool isDead_ = false;
};

