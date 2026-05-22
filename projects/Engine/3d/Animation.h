#pragma once
#include <map>
#include <string>
#include "Struct.h"

namespace YKEngine
{
class BaseModel;

/// <summary>
/// 3Dモデルのアニメーションを管理するクラス。
/// アニメーションデータを読み込み、アニメーションの再生を行う。
/// アニメーションはNode単位で管理する。
/// </summary>
class Animation
{
public:

	/// <summary>
	/// アニメーションファイルを読み込む。
	/// </summary>
	/// <param name="directoryPath">アニメーションファイルが存在するディレクトリのパス</param>
	/// <param name="filename">アニメーションファイル名</param>
	void LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// アニメーションを更新する。
	/// </summary>
	void Update(bool isLoop);

	/// <summary>
	/// RootNodeのアニメーションを再生する。
	/// </summary>
	/// <param name="model">アニメーションを適用するモデル</param>
	/// <returns>変換行列</returns>
	Matrix4x4 Reproducing(BaseModel* model);

	/// <summary>
	/// Keyframeから補間して値を計算する。
	/// </summary>
	/// <param name="keyframes">Keyframeの配列</param>
	/// <param name="time">現在のアニメーション時間</param>
	/// <returns>補間後の値</returns>
	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	
	/// <summary>
	/// Keyframeから補間して値を計算する。
	/// </summary>
	/// <param name="keyframes">Keyframeの配列</param>
	/// <param name="time">現在のアニメーション時間</param>
	/// <returns>補間後の値</returns>
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	const std::map<std::string, NodeAnimation>& GetNodeAnimations() const { return nodeAnimations_; }

	float GetAnimationTime() { return animationTime_; }

	void SetAnimationTime(float time) { animationTime_ = time; }

	float GetDuration() const { return duration_; }

private:

	float duration_;	//アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合。Node名で引けるようにしておく
	std::map<std::string, NodeAnimation> nodeAnimations_;
	
	float animationTime_;	//再生中の時刻(単位は秒)

};

} // namespace YKEngine