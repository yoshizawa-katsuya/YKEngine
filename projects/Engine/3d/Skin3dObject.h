#pragma once
#include "Base3dObject.h"

/// <summary>
/// スキンモデルを扱うクラス。
/// Base3dObjectを継承している。
/// スキンメッシュを持つモデルを描画するために使用される。
/// スキニングアニメーションをサポートしており、ボーンとジョイントの管理も行う。
/// ボーンとジョイントのデバッグ描画機能も備えている。
/// </summary>
class Skin3dObject : public Base3dObject
{
public:

	~Skin3dObject();

	void Initialize(BaseModel* model) override;

	//void Update(const WorldTransform& worldTransform, Camera* camera) override;

	//void Update(const WorldTransform& worldTransform, Camera* camera, Animation* animation) override;

	void AnimationUpdate(Animation* animation) override;

	void Draw() override;

	void Draw(uint32_t textureHandle) override;

	//デバッグ機能
	void BoneDraw(const EulerTransform& transform, Camera* camera);

	//デバッグ機能
	void JointDraw(const EulerTransform& transform, Camera* camera);

private:

	void CreateSkelton();

	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent);

	void CreateSkinCluster();

	void ApplyAnimation(Animation* animation);

	void SkeletonUpdate();

	void SkinClusterUpdate();

	Skeleton skeleton_;

	SkinCluster skinCluster_;


};

