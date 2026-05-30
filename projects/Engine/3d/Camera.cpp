#include "Camera.h"
#include "Matrix.h"
#include <numbers>
#include "GlobalVariables.h"
#include "JsonKeys.h"

using namespace YKEngine;

Camera::Camera()
	: transform_({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} })
	, aspectRatio_(static_cast<float>(WinApp::kClientWidth) / static_cast<float>(WinApp::kClientHeight))
	, worldMatrix_(MakeAffineMatrix(transform_.scale, transform_.rotation, transform_.translation))
	, viewMatrix_(Inverse(worldMatrix_))
	, backToFrontMatrix_(MakeRotateYMatrix(std::numbers::pi_v<float>))
	, dxCommon_(DirectXCommon::GetInstance())
	, cameraResource_(dxCommon_->CreateBufferResource(sizeof(CameraForGPU)))
{
	//jsonから値を取得してメンバ変数に記録する
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const std::string& groupName = JsonKey::Camera::kGroupName;
	fovY_ = globalVariables->GetFloatValue(groupName, JsonKey::Camera::kFovY);
	nearClip_ = globalVariables->GetFloatValue(groupName, JsonKey::Camera::kNearClip);
	farClip_ = globalVariables->GetFloatValue(groupName, JsonKey::Camera::kFarClip);

	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

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

void Camera::SetCameraReaource(uint32_t rootParamIndex)
{

	cameraForGPUData_->worldPosition = transform_.translation;

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(rootParamIndex, cameraResource_->GetGPUVirtualAddress());

}

Matrix4x4 Camera::MakeBillBoardMatrix()
{
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix_, worldMatrix_);
	billboardMatrix.m[3][0] = 0.0f;	//平行移動成分はいらない
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	return billboardMatrix;
}
