#pragma once
#include "Struct.h"
#include "WinApp.h"
#include "DirectXCommon.h"

//カメラ
class Camera
{
public:

	Camera();

	//更新
	void Update();

	void SetCameraReaource();

	//getter
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjection() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjection() const { return viewProjectionMatrix_; }

	const Vector3& GetRotate() const { return transform_.rotation; }
	Vector3& GetRotate() { return transform_.rotation; }

	const Vector3& GetTranslate() const { return transform_.translation; }
	Vector3& GetTranslate() { return transform_.translation; }

	//setter
	void SetRotate(const Vector3& rotate) { transform_.rotation = rotate; }
	void SetRotateX(float x) { transform_.rotation.x = x; }
	void SetRotateY(float y) { transform_.rotation.y = y; }
	void SetRotateZ(float z) { transform_.rotation.z = z; }

	void SetTranslate(const Vector3& translate) { transform_.translation = translate; }
	void SetFovY(float fovY) { fovY_ = fovY; }
	void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(float nearClip) { nearClip_ = nearClip; }
	void SetFarClip(float farClip) { farClip_ = farClip; }

	void SetWorldMatrix(const Matrix4x4& worldmatrix) { worldMatrix_ = worldmatrix; }
	void SetViewMatrix(const Matrix4x4& viewmatrix) { viewMatrix_ = viewmatrix; }
	void SetviewProjection(const Matrix4x4& viewProjectionMatrix) { viewProjectionMatrix_ = viewProjectionMatrix; }

private:

	struct CameraForGPU {
		Vector3 worldPosition;
	};

	//カメラの変数
	EulerTransform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	float fovY_;	//水平方向視野角
	float aspectRatio_;	//アスペクト比
	float nearClip_;	//ニアクリップ距離
	float farClip_;	//ファークリップ距離
	Matrix4x4 viewProjectionMatrix_;

	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_;
	CameraForGPU* cameraForGPUData_;
	
};
