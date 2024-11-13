#pragma once
#include "BaseModel.h"

class SkinModel : public BaseModel
{
public:

	~SkinModel() override;

	void CreateModel(const std::string& directoryPath, const std::string& filename) override;

	void Update() override;

	void Update(Animation* animation) override;

	void Draw(const WorldTransform& worldTransform, Camera* camera) override;

	void Draw(const WorldTransform& worldTransform, Camera* camera, uint32_t textureHandle) override;

	//デバッグ機能
	void BoneDraw(const EulerTransform& transform, Camera* camera);

	//デバッグ機能
	void JointDraw(const EulerTransform& transform, Camera* camera);

private:

	void CreateSkinCluster();

	void LoadModelFile(const std::string& directoryPath, const std::string& filename) override;

	//スキンクラスター作成用のデータ読み込み
	void LoadSkinCluster(aiMesh* mesh);

	void ApplyAnimation(Animation* animation);

	void SkeletonUpdate();

	void SkinClusterUpdate();

	void CreateSkelton();

	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent);

	Skeleton skeleton_;

	SkinCluster skinCluster_;

};

