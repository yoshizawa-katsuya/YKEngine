#pragma once
#include <map>
#include <string>
#include "Struct.h"
class BaseModel;

class Animation
{
public:

	void LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	//animationTimeを進める
	void Update();

	//RootNodeのアニメーションを再生する
	Matrix4x4 Reproducing(BaseModel* model);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	const std::map<std::string, NodeAnimation>& GetNodeAnimations() const { return nodeAnimations_; }

	//const NodeAnimation& GetNodeAnimation(const std::string& nodeName) const { return nodeAnimations_.find(nodeName); }

	float GetAnimationTime() { return animationTime_; }

private:

	float duration_;	//アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合。Node名で引けるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations_;
	
	float animationTime_;	//再生中の時刻(単位は秒)

};

