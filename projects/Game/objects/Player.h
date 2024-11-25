#pragma once
#include "BaseModel.h"
#include "SkinModel.h"
#include "WorldTransform.h"
#include "Animation.h"
#include "Input.h"
#include "PlayerBullet.h"
class Camera;
class MapChipField;

class Player
{
public:
	~Player();

	void Initialize(const std::vector<BaseModel*>& models);

	void Update();

	void Draw(Camera* camera);

	void Attack();

	// ワールド座標を取得
	//Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:

	// 各部位のTransform変数を格納する
	std::vector<WorldTransform> worldTransforms_;

	std::vector<BaseModel*> models_;

	// 弾丸のモデルを生成
	std::shared_ptr<BaseModel> bulletModel;

	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	// 弾の発射地点
	WorldTransform bulletEmitter{};

	// クールタイム（秒）
	const float kCoolDownTime = 15.0f;

	float fireCoolTime = 0.0f;
};

