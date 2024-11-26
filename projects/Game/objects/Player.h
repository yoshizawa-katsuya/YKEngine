#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Animation.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Audio.h"
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

	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const;

	// 死亡フラグのゲッター
	bool IsDead() { return isDead_; }

private:

	// 各部位のTransform変数を格納する
	std::vector<WorldTransform> worldTransforms_;

	// 弾丸のモデルを生成
	std::unique_ptr<BaseModel> bulletModel;
	
	Audio* audio_;
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	// 弾の発射地点
	WorldTransform bulletEmitter{};


	// クールタイム（秒）
	const float kCoolDownTime = 15.0f;

	float fireCoolTime = 0.0f;

	//オブジェクト
	std::vector<std::unique_ptr<Base3dObject>> objects_;

	std::unique_ptr<Base3dObject> bulletObject;

	// プレイヤーの現在体力
	uint32_t playerHP;
	// プレイヤーの最大体力
	const uint32_t playerMaxHP = 10;

	// プレイヤーの死亡フラグ
	bool isDead_;

	//se
	SoundData hitSE01_;

};

