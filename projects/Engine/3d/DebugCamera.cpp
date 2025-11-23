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
	//マウス左クリックで注視点を設定
	if (input_->TrigerMouseLeft()) 
	{
		Vector3 offset = { 0.0f, 0.0f, 10.0f };
		target_ = camera_->GetTranslate() + TransformNormal(offset, camera_->GetWorldMatrix());
	}

	//マウス中央クリックでカメラ移動
	if (input_->PushMouseCenter()) 
	{
		Vector2 mousevelocity = input_->GetMouseVelocity();

		const float speed = 0.01f;

		//カメラ移動ベクトル。xy平面移動
		Vector3 move = { speed, speed, 0.0f };
		move.x *= -mousevelocity.x;
		move.y *= mousevelocity.y;
		move = TransformNormal(move, camera_->GetWorldMatrix());

		camera_->SetTranslate(camera_->GetTranslate() + move);
	}
	//マウス左クリックで注視点を中心に回転
	else if (input_->PushMouseLeft()) 
	{

		Vector2 mousevelocity = input_->GetMouseVelocity();

		const float speed = 0.001f;

		//カメラ回転ベクトル。xy平面回転
		Vector3 move = { speed, speed, 0 };
		move.x *= mousevelocity.y;
		move.y *= mousevelocity.x;

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
	//マウスホイールで前後移動
	else 
	{
		const float speed = 0.01f;

		//カメラ移動ベクトル。Z軸移動
		Vector3 move = { 0, 0, speed };
		move.z *= input_->GetMouseWheel();
		move = TransformNormal(move, camera_->GetWorldMatrix());

		camera_->SetTranslate(camera_->GetTranslate() + move);
	}
	//ZキーでZ軸回転
	if (input_->PushKey(DIK_Z)) 
	{
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