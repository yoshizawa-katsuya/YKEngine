#include "PressA.h"
#include "Sprite.h"
#include "Lerp.h"
#include "Easing.h"

using namespace YKEngine;

void PressA::Initialize()
{
	spritePressA_ = std::make_unique<Sprite>();
	spritePressA_->Initialize(TextureManager::GetInstance()->Load("./Resources/pressA.png"));
}

void PressA::Update()
{
	t_ += 1.0f / 60.0f; // 60FPSで更新されることを想定

	//t_が1を超えないようにする
	if (t_ > 1.0f)
	{
		t_ = 1.0f;
	}

	//PressAを徐々に表示/非表示にする
	if (isPressAVisible_)
	{
		spritePressA_->SetColor({ 1.0f, 1.0f, 1.0f, Lerp(0.0f, 1.0f, EaseOutSine(t_)) });
	}
	else
	{
		spritePressA_->SetColor({ 1.0f, 1.0f, 1.0f, Lerp(1.0f, 0.0f, EaseInSine(t_)) });
	}

	//t_が1を超えたら、Aの表示/非表示を切り替える
	if (t_ >= 1.0f)
	{
		t_ = 0.0f;
		isPressAVisible_ = !isPressAVisible_;
	}
}

void PressA::Draw()
{
	spritePressA_->Draw();
}

void PressA::SetPosition(Vector2 position)
{
	spritePressA_->SetPosition(position);
}
