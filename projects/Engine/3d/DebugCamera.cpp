#include "DebugCamera.h"
#include "Camera.h"
#include "Input.h"
#include "Matrix.h"

void DebugCamera::Initialize(Camera* camera, Input* input)
{

	camera_ = camera;
	input_ = input;

	matRot_ = MakeIdentity4x4();

}

void DebugCamera::Update()
{

	if (input_->TrigerMouseLeft()) {

		target_ = camera_->GetTranslate() + TransformNormal(Vector3{0.0f, 0.0f, 10.0f}, camera_->GetWorldMatrix());

		//target_ = TransformNormal(target_, camera_->GetWorldMatrix());
	}

	if (input_->PushMouseCenter()) {
		Vector2 mousevelocity = input_->GetMouseVelocity();

		const float speed = 0.01f;

		//カメラ移動ベクトル
		Vector3 move = { speed, speed, 0 };
		move.x *= -mousevelocity.x;
		move.y *= mousevelocity.y;
		move = TransformNormal(move, camera_->GetWorldMatrix());

		camera_->SetTranslate(camera_->GetTranslate() + move);
	}

	else if (input_->PushMouseLeft()) {

		Vector2 mousevelocity = input_->GetMouseVelocity();

		const float speed = 0.001f;

		//カメラ移動ベクトル
		Vector3 move = { speed, speed, 0 };
		move.x *= mousevelocity.y;
		move.y *= mousevelocity.x;
		
		//camera_->SetRotateX(camera_->GetRotate().x + move.x);
		//camera_->SetRotateY(camera_->GetRotate().y + move.y);

		Matrix4x4 matRotDelta = MakeIdentity4x4();
		matRotDelta = matRotDelta * MakeRotateXMatrix(move.x);
		matRotDelta = matRotDelta * MakeRotateYMatrix(move.y);

		//累積の回転行列を合成
		matRot_ = matRotDelta * matRot_;

		camera_->Update();
		ViewMatrixUpdate();

		Vector3 offset = { 0.0f, 0.0f, -10.0f };

		offset = TransformNormal(offset, camera_->GetWorldMatrix());

		camera_->SetTranslate(target_ + offset);

	}

	else {
		const float speed = 0.01f;

		//カメラ移動ベクトル
		Vector3 move = { 0, 0, speed };
		move.z *= input_->GetMouseWheel();
		move = TransformNormal(move, camera_->GetWorldMatrix());

		camera_->SetTranslate(camera_->GetTranslate() + move);
	}
	
	if (input_->PushKey(DIK_Z)) {
		const float speed = 0.01f;

		camera_->SetRotateZ(camera_->GetRotate().z + speed);
	}

	camera_->Update();
	ViewMatrixUpdate();
}

void DebugCamera::ViewMatrixUpdate()
{

	Matrix4x4 worldmatrix = Multiply(matRot_, MakeTranslateMatrix(camera_->GetTranslate()));
	camera_->SetWorldMatrix(worldmatrix);

	Matrix4x4 viewMatrix = Inverse(worldmatrix);
	camera_->SetViewMatrix(viewMatrix);

	camera_->SetviewProjection(Multiply(viewMatrix, camera_->GetProjection()));

}

void DebugCamera::AddRotateX(float rotateX)
{
	Matrix4x4 matRotDelta = MakeIdentity4x4() * MakeRotateXMatrix(rotateX);
	
	//累積の回転行列を合成
	matRot_ = matRotDelta * matRot_;
}

void DebugCamera::AddRotateY(float rotateY)
{
	Matrix4x4 matRotDelta = MakeIdentity4x4() * MakeRotateXMatrix(rotateY);

	//累積の回転行列を合成
	matRot_ = matRotDelta * matRot_;
}

void DebugCamera::AddRotateZ(float rotateZ)
{
	Matrix4x4 matRotDelta = MakeIdentity4x4() * MakeRotateXMatrix(rotateZ);

	//累積の回転行列を合成
	matRot_ = matRotDelta * matRot_;
}
