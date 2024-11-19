#include "LockOnSystem.h"
#include "Camera.h"

void LockOnSystem::Initialize()
{
	viewProjection_ = camera_->GetViewProjection();
	matView_ = camera_->GetViewMatrix();
}
