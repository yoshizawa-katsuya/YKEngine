#pragma once
#include "SphereCollider.h"
#include "Skin3dObject.h"

/// <summary>
/// スキンモデルを使用するキャラクターの基底クラス。
/// </summary>
class SkinCharacter : public SphereCollider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SkinCharacter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="animation">アニメーション</param>
	virtual void Initialize(YKEngine::BaseModel* model);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">描画に使用するカメラ</param>
	virtual void Draw(YKEngine::Camera* camera);

	bool IsDead() const { return isDead_; }

protected:

	std::unique_ptr<YKEngine::Skin3dObject> object_;
	//ワールド変換データ
	YKEngine::WorldTransform characterWorldTransform_;

	//デスフラグ
	bool isDead_ = false;
};

