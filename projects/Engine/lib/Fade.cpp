#include "Fade.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include <algorithm>

Fade::~Fade() {

	delete sprite_;

}

void Fade::Initialize() {

	uint32_t textureHandle = TextureManager::GetInstance()->Load("./resources/white.png");

	sprite_ = new Sprite();
	sprite_->Initialize(textureHandle);
	sprite_->SetSize(Vector2(1280.0f, 720.0f));
	sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

}

void Fade::Update() {

	//フェード状態による分岐
	switch (status_) {
	case Fade::Status::None:
		//何もしない
		break;
	case Fade::Status::FadeIn:

		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を小さくする
		sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(1.0f - (counter_ / duration_), 0.0f, 1.0f)));


		break;
	case Fade::Status::FadeOut:

		//1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		//フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		//0.0fから1.0fの間で、経過時間がフェード持続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	default:
		break;
	}

}

void Fade::Draw() {

	if (status_ == Status::None) {
		return;
	}

	sprite_->Draw();

}

void Fade::Start(Status status, float duration) {

	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;

}

void Fade::Stop() {

	status_ = Status::None;

}

bool Fade::IsFinished() { 
	
	//フェード状態による分岐
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:

		return counter_ >= duration_;

		break;
	}

	return true;

}
