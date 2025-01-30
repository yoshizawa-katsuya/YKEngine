#include "Ground.h"
#include "cassert"

void Ground::Initialize(Model* model, ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Ground::Update() { 
	worldTransform_.TransferMatrix();
}

void Ground::Draw() {
	model_->Draw(worldTransform_, *viewProjection_); 
}