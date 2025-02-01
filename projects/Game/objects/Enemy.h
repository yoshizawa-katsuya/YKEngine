#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter{
public:

	//デフォルトコンストラクタ
	Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="objects_">モデルデータ配列</param>
	void Initialize(const std::vector<RigidModel*>& models, RigidModel* colliderModel) override;

	// 腕振りギミック初期化
	void InitializeRollArmGimmick();

	//ヒットエフェクト初期化
	void InitializeHitEffect(RigidModel* model);

	/// <summary>
	/// 更新
	///  </summary>
	void Update() override;

	// 腕振りギミック更新
	void UpdateRollArmGimmick();

	//ヒットエフェクト更新
	void UpdateHitEffect();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw(Camera* camera) override;

	//ヒットエフェクト描画
	void DrawHitEffect(Camera* camera);

	//ヒット
	void Hit();

	//中心座標を取得
	Vector3 GetCenterPosition() const override;

	//シリアルナンバーを取得
	uint32_t GetSerialNumber() const { return serialNumber_; }

private:

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHitEffect_;

	const uint16_t kModelIndexBody = 0;
	const uint16_t kModelIndexL_arm = 1;
	const uint16_t kModelIndexR_arm = 2;

	// 腕振りギミック用の媒介変数
	float rollArmParameter_ = 0.0f;

	//ヒットエフェクトの媒介変数
	float hitEffectParameter_ = 0.0f;

	//ヒットエフェクトモデル
	std::unique_ptr<Rigid3dObject> objectHitEffect_;

	//シリアルナンバー
	uint32_t serialNumber_ = 0;

	//次のシリアルナンバー
	static uint32_t nextSerialNumber_;

};
