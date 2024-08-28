#pragma once
#include "Struct.h"
#include "WinApp.h"

//カメラ
class Camera
{
public:

	Camera();

	//更新
	void Update();

	//getter
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjection() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjection() const { return viewProjectionMatrix_; }

	const Vector3& GetRotate() const { return transform_.rotate; }
	Vector3& GetRotate() { return transform_.rotate; }

	const Vector3& GetTranslate() const { return transform_.translate; }
	Vector3& GetTranslate() { return transform_.translate; }

	//setter
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetRotateX(float x) { transform_.rotate.x = x; }
	void SetRotateY(float y) { transform_.rotate.y = y; }
	void SetRotateZ(float z) { transform_.rotate.z = z; }

	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	void SetFovY(float fovY) { fovY_ = fovY; }
	void SetAspectRatio(float aspectRatio) { aspectRatio_ = aspectRatio; }
	void SetNearClip(float nearClip) { nearClip_ = nearClip; }
	void SetFarClip(float farClip) { farClip_ = farClip; }

	void SetWorldMatrix(const Matrix4x4& worldmatrix) { worldMatrix_ = worldmatrix; }
	void SetViewMatrix(const Matrix4x4& viewmatrix) { viewMatrix_ = viewmatrix; }
	void SetviewProjection(const Matrix4x4& viewProjectionMatrix) { viewProjectionMatrix_ = viewProjectionMatrix; }

private:

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

};

