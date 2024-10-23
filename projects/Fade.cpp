#include "Fade.h"
#include "algorithm"

void Fade::Initialize()
{
	spritePlatform_ = SpritePlatform::GetInstance();

	curtainSprite_ = TextureManager::GetInstance()->Load("./resources/white.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(curtainSprite_, spritePlatform_);
	sprite_->SetPosition({ 250.0f, 360.0f });
}

void Fade::Start(Status status, float duration)
{

	sprite_->SetTextureSize(Vector2(720.0f, 500.0f));
	sprite_->SetAnchorPoint(Vector2(0.5f, 0.5f));



	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Update()
{
	switch (status_) {
	case Status::None:

		break;

	case Status::FadeIn:

		// 1フレーム分カウント
		counter_ += 1.0f / 60.0f;

		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を小さくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1 - counter_ / duration_, 0.0f, 1.0f)));

		break;

	case Status::FadeOut:

		counter_ += 1.0f / 60.0f;

		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	}
	

}

void Fade::Draw()
{

	sprite_->Draw();

}
