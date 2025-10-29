#include "DirectionalLight.h"

void DirectionalLight::Initialize()
{
	
	//デフォルト値
	data_ = new DirectionalLightData();
	data_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	data_->direction = { -0.3f, -1.0f, -0.3f };
	data_->intensity = 1.0f;

}
