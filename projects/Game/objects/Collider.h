#pragma once
#include "WorldTransform.h"
#include "CollisionTypeIdDef.h"
class Camera;

/// <summary>
/// 衝突判定オブジェクト。
/// 継承して使用する。
/// 衝突判定は球同士の当たり判定で行う。
/// 衝突時にはOnCollision関数が呼ばれる。
/// 衝突判定を行うオブジェクトはColliderManagerに登録すること。
/// </summary>
class Collider
{
public:

	/// <summary>
	/// デストラクタ。純粋仮想関数。
	/// </summary>
	virtual ~Collider() = default;

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のCollider</param>
	virtual void OnCollision([[maybe_unused]] Collider* other) {}

	/// <summary>
	/// カメラに映っているかどうか。
	/// </summary>
	/// <param name="camera">判定に使用するカメラ</param>
	/// </returns>映っているならtrue、映っていないならfalse</returns>
	virtual bool IsVisible(Camera* camera);

	/// <summary>
	/// 中心座標を取得。
	/// </summary>
	/// <returns>中心座標</returns>
	virtual Vector3 GetCenterPosition();

	//半径を取得
	float GetRadius() { return radius_; }

	void SetRadius(float radius);

	//種別IDを取得
	CollisionTypeIdDef GetTypeID() const { return typeID_; }

	//種別IDを設定
	void SetTypeID(CollisionTypeIdDef typeID) { typeID_ = typeID; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

protected:

	Vector4 ClipPosition(Camera* camera);

	//Transform変数を作る
	WorldTransform worldTransform_;

	//衝突半径
	float radius_ = 1.0f;

	//種別ID
	CollisionTypeIdDef typeID_ = CollisionTypeIdDef::kDefault;
};

