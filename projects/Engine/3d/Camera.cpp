#include "Camera.h"
#include "Matrix.h"
#include <numbers>

Camera::Camera()
	: transform_({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} })
	, fovY_(0.45f)
	, aspectRatio_(static_cast<float>(WinApp::kClientWidth) / static_cast<float>(WinApp::kClientHeight))
	, nearClip_(0.1f)
	, farClip_(100.0f)
	, worldMatrix_(MakeAffineMatrix(transform_.scale, transform_.rotation, transform_.translation))
	, viewMatrix_(Inverse(worldMatrix_))
	, projectionMatrix_(MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_))
	, viewProjectionMatrix_(Multiply(viewMatrix_, projectionMatrix_))
	, backToFrontMatrix_(MakeRotateYMatrix(std::numbers::pi_v<float>))
	, dxCommon_(DirectXCommon::GetInstance())
	, cameraResource_(dxCommon_->CreateBufferResource(sizeof(CameraForGPU)))
{
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPUData_));
	cameraForGPUData_->worldPosition = transform_.translation;
}

void Camera::Update()
{

	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotation, transform_.translation);
	viewMatrix_ = Inverse(worldMatrix_);

	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::SetCameraReaource()
{

	cameraForGPUData_->worldPosition = transform_.translation;

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResource_->GetGPUVirtualAddress());

}

Matrix4x4 Camera::MakeBillBoardMatrix()
{
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix_, worldMatrix_);
	billboardMatrix.m[3][0] = 0.0f;	//平行移動成分はいらない
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	return billboardMatrix;
}
