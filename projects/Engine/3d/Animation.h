#pragma once
#include <map>
#include <string>
#include "Struct.h"
class Model;

class Animation
{
public:

	void LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	Matrix4x4 Reproducing(Model* model);

private:

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	float duration_;	//アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合。Node名で退けるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations_;
	
	float animationTime_;	//再生中の時刻(単位は秒)

};

