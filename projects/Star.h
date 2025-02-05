#pragma once
#include "Base3dObject.h"
#include "WorldTransform.h"
#include "Input.h"
#include <imgui.h>

class Star
{
public:

	void Initialize(BaseModel* model_, const Vector3& position);
	void Update();
	void Draw(Camera* camera);

	Vector3 GetPosition()const { return worldTransform_.translation_; }
private:
	WorldTransform worldTransform_;
	std::unique_ptr<Base3dObject> object_;
	Input* input_ = nullptr;
};
