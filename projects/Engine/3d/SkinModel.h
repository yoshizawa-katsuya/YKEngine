#pragma once
#include "BaseModel.h"

class SkinModel : public BaseModel
{
public:

	void CreateModel(const std::string& directoryPath, const std::string& filename) override;

	//アニメーションを適用する場合はApplyAnimationをUpdateの前に呼び出す
	void Update() override;

	void ApplyAnimation(Animation* animation);

private:

	void CreateSkinCluster();

	void SkeletonUpdate();

	void SkinClusterUpdate();

	void BoneDraw(const EulerTransform& transform, Camera* camera);

	void JointDraw(const EulerTransform& transform, Camera* camera);

	

};

