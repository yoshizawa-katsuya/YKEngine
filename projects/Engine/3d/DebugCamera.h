#pragma once
#include "Struct.h"
#include <memory>

namespace YKEngine
{
class Camera;
class Input;

/// <summary>
/// デバッグ用カメラ。
/// マウスで操作可能。
/// </summary>
class DebugCamera
{
public:

	/// <summary>
	/// 初期化。
	/// デバッグカメラとして使用するカメラと入力を指定する。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	Camera* GetCamera() { return camera_.get(); }

private:

	/// <summary>
	/// ビュー行列の更新。
	/// </summary>
	void ViewMatrixUpdate();

	std::unique_ptr<Camera> camera_;
	Input* input_;

	Matrix4x4 matRot_;

	Vector3 target_ = { 0.0f, 0.0f, 10.0f };

};

} // namespace YKEngine