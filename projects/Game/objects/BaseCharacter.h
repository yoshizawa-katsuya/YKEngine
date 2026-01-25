#pragma once
#include "Collider.h"
#include "My3dObject.h"

/// <summary>
/// キャラクターの基底クラス。
/// 継承して使用する。
/// キャラクターはコライダーを持つためColliderクラスを継承する。
/// </summary>
class BaseCharacter : public Collider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseCharacter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
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

	std::unique_ptr<YKEngine::My3dObject> object_;
	//ワールド変換データ
	YKEngine::WorldTransform characterWorldTransform_;

	//デスフラグ
	bool isDead_ = false;
};

