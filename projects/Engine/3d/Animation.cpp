#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cassert>
#include "Lerp.h"
#include "Slerp.h"
#include "BaseModel.h"
#include "Matrix.h"

using namespace YKEngine;

void Animation::LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);

	std::string logMsg = "[Anim Load] Loading: " + filename + "\n";
	OutputDebugStringA(logMsg.c_str());

	assert(scene->mNumAnimations != 0);	//アニメーションがない
	// 1. 拡張子を抜いたターゲットのアニメーション名を作る (例: "Stay.gltf" -> "Stay")
	std::string targetAnimName = filename;
	size_t dotPos = targetAnimName.find_last_of('.');
	if (dotPos != std::string::npos) {
		targetAnimName = targetAnimName.substr(0, dotPos);
	}

	aiAnimation* animationAssimp = nullptr;

	// 2. ファイル内にある全アニメーションをループして、名前が一致するものを探す
	for (uint32_t i = 0; i < scene->mNumAnimations; ++i) {
		std::string currentAnimName = scene->mAnimations[i]->mName.C_Str();

		// 完全一致、またはBlender特有の "Armature|Stay" のような命名に対応するため部分一致で検索
		if (currentAnimName.find(targetAnimName) != std::string::npos) {
			animationAssimp = scene->mAnimations[i];

			std::string foundLog = "  -> [Success] Match found: " + currentAnimName + "\n";
			OutputDebugStringA(foundLog.c_str());
			break;
		}
	}

	// 3. 万が一見つからなかった場合の安全弁として、従来通り0番目を使う
	if (!animationAssimp) {
		animationAssimp = scene->mAnimations[0];
		std::string failLog = "  -> [Warning] Target not found. Using default index 0: " + std::string(animationAssimp->mName.C_Str()) + "\n";
		OutputDebugStringA(failLog.c_str());
	}
	
	std::string nameLog = "  -> [Actual Anim Name in File]: " + std::string(animationAssimp->mName.C_Str()) + "\n";
	OutputDebugStringA(nameLog.c_str());

	duration_ = static_cast<float>(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	//時間の単位を秒に変換

	//assimpでは個々のNodeのAnimationをchannelと呼んでいるのでchannelを回してNodeAnimationの情報をとってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = nodeAnimations_[nodeAnimationAssimp->mNodeName.C_Str()];
		//位置、回転、拡縮のキーフレームをそれぞれ登録していく
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) 
		{
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = static_cast<float>(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//ここも秒に変換
			keyframe.value = { -keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };	//右手→左手
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}

		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) 
		{
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;
			keyframe.time = static_cast<float>(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };	//右手→左手
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}

		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex)
		{
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = static_cast<float>(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };	//右手→左手
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}

	}

}

void Animation::Update(bool isLoop)
{
	animationTime_ += 1.0f / 60.0f; // 時刻を進める

	if (isLoop) {
		// ループする場合は従来通り fmod
		animationTime_ = std::fmod(animationTime_, duration_);
	}
	else {
		// ループしない（固定ポーズやReturnなど）場合は、末尾で時間を止める（クランプ）
		if (animationTime_ > duration_) {
			animationTime_ = duration_;
		}
	}
}

Matrix4x4 Animation::Reproducing(BaseModel* model)
{
	
	NodeAnimation& rootNodeAnimation = nodeAnimations_[model->GetRootNode().name];	//rootNodeのAnimationを取得
	Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);//指定時刻の値を取得
	Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
	Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
	return MakeAffineMatrix(scale, rotate, translate);
}

Vector3 Animation::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
	
	assert(!keyframes.empty());	//キーがない物は返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time) {	//キーが1つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			//範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	//ここまで来た場合は一番後の時刻鳥も後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}

Quaternion Animation::CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time)
{

	assert(!keyframes.empty());	//キーがない物は返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time) {	//キーが1つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			//範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	//ここまで来た場合は一番後の時刻鳥も後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;

}