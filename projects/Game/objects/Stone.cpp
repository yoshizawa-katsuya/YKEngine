#include "Stone.h"

Stone::Stone()
	:input_(Input::GetInstance())
{
}

void Stone::Initialize(BaseModel* model, const Vector3& position)
{
	object_ = std::make_unique<Rigid3dObject>();
	object_->Initialize(model);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Stone::Update()
{
	Vector2 mousePos = input_->GetMousePosition();

	if (input_->PushMouseLeft()) {
		Vector3 clickPos = ConvertScreenToWorld(mousePos);

		float distance = static_cast<float>(sqrt(pow(clickPos.x - worldTransform_.translation_.x, 2) + pow(clickPos.z - worldTransform_.translation_.z, 2)));

		if (distance < 1.0f) {
			isDragging_ = true;
			dragStartPos_ = mousePos;
		}
	}
	if (isDragging_ && input_->PushMouseLeft()) {
		dragCurrentPos_ = input_->GetMousePosition();
	}
	if (isDragging_ && !input_->PushMouseLeft() && !input_->TrigerMouseLeft()) {
		isDragging_ = false;
		Vector2 dragVector = { dragStartPos_.x - dragCurrentPos_.x  , dragStartPos_.y - dragCurrentPos_.y };

		float length = sqrt(dragVector.x * dragVector.x + dragVector.y * dragVector.y);
		if (length > 0) {
			dragVector.x /= length;
			dragVector.y /= length;
		}

		float speed = std::min(length * 0.03f, maxSpeed_);
		velocity_ = { dragVector.x * speed, 0.0f, -dragVector.y * speed };

	}


	worldTransform_.translation_ += velocity_;
	velocity_ *= friction_;

	if (fabs(velocity_.x) < 0.01f) velocity_.x = 0.0f;
	if (fabs(velocity_.z) < 0.01f) velocity_.z = 0.0f;

	
	worldTransform_.UpdateMatrix();
	object_->WorldTransformUpdate(worldTransform_);
}

void Stone::Draw(Camera* camera)
{
	object_->CameraUpdate(camera);
	object_->Draw();
}

Vector3 Stone::ConvertScreenToWorld(const Vector2& screenPos)
{
	float worldX = (screenPos.x / 990.0f) * 34.0f - 17.0f;
	float worldZ = (screenPos.y / 600.0f) * 30.0f - 15.0f;

	return Vector3(worldX, 0.0f, -worldZ);
}
