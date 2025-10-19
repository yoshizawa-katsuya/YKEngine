#pragma once
#include "Struct.h"
#include "WinApp.h"
#include "DirectXCommon.h"

/// <summary>
/// カメラクラス。
/// 3D空間上の視点を表す。
/// 視点の位置、向き、投影方法を管理する。
/// </summary>
class Camera
{
public:

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Camera();

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// ConstantBufferにカメラの情報をセットする。
	/// </summary>
	void SetCameraReaource();

	/// <summary>
	/// ビルボード行列の作成。
	/// </summary>
	/// <returns>ビルボード行列</returns>
	Matrix4x4 MakeBillBoardMatrix();

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

	/// <summary>
	/// GPU用カメラ構造体。
	/// </summary>
	struct CameraForGPU 
	{
		Vector3 worldPosition;
	};

	//カメラの変数
	float fovY_;	//垂直方向視野角
	float aspectRatio_;	//アスペクト比
	float nearClip_;	//ニアクリップ距離
	float farClip_;	//ファークリップ距離
	EulerTransform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

	Matrix4x4 backToFrontMatrix_;

	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_;
	CameraForGPU* cameraForGPUData_;
	
};

