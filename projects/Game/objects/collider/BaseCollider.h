#pragma once
#include "WorldTransform.h"
#include "CollisionTypeIdDef.h"
#include "ColliderShapeType.h"

namespace YKEngine
{
	class Camera;
}

/// <summary>
/// 衝突判定オブジェクト。
/// 継承して使用する。
/// 衝突判定は球同士の当たり判定で行う。
/// 衝突時にはOnCollision関数が呼ばれる。
/// 衝突判定を行うオブジェクトはCollisionManagerに登録すること。
/// </summary>
class BaseCollider
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	virtual ~BaseCollider() = default;

	/// <summary>
	/// 初期化。
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// 衝突時の処理。
	/// </summary>
	/// <param name="other">衝突相手のCollider</param>
	virtual void OnCollision([[maybe_unused]] BaseCollider* other) {}

	/// <summary>
	/// カメラに映っているかどうか。
	/// </summary>
	/// <param name="camera">判定に使用するカメラ</param>
	/// <returns>映っているならtrue、映っていないならfalse</returns>
	virtual bool IsVisible(YKEngine::Camera* camera);

	/// <summary>
	/// 中心座標を取得。
	/// </summary>
	/// <returns>中心座標</returns>
	virtual YKEngine::Vector3 GetCenterPosition();

	//種別IDを取得
	CollisionTypeIdDef GetTypeID() const { return typeID_; }

	//種別IDを設定
	void SetTypeID(CollisionTypeIdDef typeID) { typeID_ = typeID; }

	const YKEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

protected:

	/// <summary>
	/// クリップ座標を取得。
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <returns>クリップ座標</returns>
	YKEngine::Vector4 ClipPosition(YKEngine::Camera* camera);

	//Transform変数を作る
	YKEngine::WorldTransform worldTransform_;

	//種別ID
	CollisionTypeIdDef typeID_ = CollisionTypeIdDef::kDefault;

	//コライダーの形状タイプ
	ColliderShapeType shapeType_;
};

