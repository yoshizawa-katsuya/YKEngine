#include "SceneChangeStaging.h"
#include "TextureManager.h"

using namespace YKEngine;

std::unique_ptr<SceneChangeStaging> SceneChangeStaging::instance_ = nullptr;

SceneChangeStaging* SceneChangeStaging::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = std::make_unique<SceneChangeStaging>(ConstructorKey());
	}
	return instance_.get();
}

void SceneChangeStaging::Finalize()
{
	//リソースリークチェックのため、明示的にインスタンスを破棄する
	instance_.reset();
}

void SceneChangeStaging::Initialize()
{
	uint32_t textureHandleSceneChange = TextureManager::GetInstance()->Load("./Resources/SceneChange01_sheet.png");

	//スプライトの生成
	spriteSceneChange_ = std::make_unique<AnimatedSprite>();
	spriteSceneChange_->Initialize(textureHandleSceneChange, 20, 3);
	spriteSceneChange_->SetSize({ WinApp::kClientWidth , WinApp::kClientHeight });
	spriteSceneChange_->SetIsLoop(false);
	spriteSceneChange_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	//フェードの初期化
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	fadeGray_ = std::make_unique<Fade>();
	fadeGray_->Initialize({ 0.2f, 0.2f, 0.2f, 1.0f });
}

void SceneChangeStaging::Update()
{
	if (!isStaging_)
	{
		return;
	}

	switch (stagingType_)
	{
	case StagingType::kFade:
		fade_->Update();
		break;
	case StagingType::kMixFade:
		fade_->Update();
		fadeGray_->Update();
		break;
	case StagingType::kEye:
		spriteSceneChange_->Update();
		break;
	default:
		break;
	}
}

void SceneChangeStaging::Draw()
{
	switch (stagingType_)
	{
	case StagingType::kFade:
		fade_->Draw();
		break;
	case StagingType::kMixFade:
		fadeGray_->Draw();
		fade_->Draw();
		break;
	case StagingType::kEye:
		spriteSceneChange_->Draw();
		break;
	default:
		break;
	}
}

void SceneChangeStaging::BeginSceneStart(StagingType stagingType)
{
	stagingType_ = stagingType;

	switch (stagingType_)
	{
	case StagingType::kFade:
		fade_->Start(Fade::Status::kFadeIn, fadeDuration_);
		fade_->Update();  // 即座に更新して真っ黒にする
		break;
	case StagingType::kMixFade:
		fade_->Start(Fade::Status::kFadeIn, fadeDuration_);
		fade_->Update();  // 即座に更新して真っ黒にする
		fadeGray_->Start(Fade::Status::kFadeIn, fadeDuration_ / 2);
		fadeGray_->Update();  // 即座に更新して真っ黒にする
		break;
	case StagingType::kEye:
		spriteSceneChange_->Reset();
		break;
	default:
		break;
	}
	isStaging_ = true;
}

void SceneChangeStaging::BeginSceneEnd(StagingType stagingType)
{
	stagingType_ = stagingType;

	switch (stagingType_)
	{
	case StagingType::kFade:
		fade_->Start(Fade::Status::kFadeOut, fadeDuration_);
		fade_->Update();  // 即座に更新してフェードが見えないようにする
		break;
	case StagingType::kMixFade:
		fade_->Start(Fade::Status::kFadeOut, fadeDuration_);
		fade_->Update();  // 即座に更新してフェードが見えないようにする
		fadeGray_->Start(Fade::Status::kFadeOut, fadeDuration_ / 2);
		fadeGray_->Update();  // 即座に更新してフェードが見えないようにする
		break;
	case StagingType::kEye:
		spriteSceneChange_->ResetReverseAnimation();
		break;
	default:
		break;
	}

	isStaging_ = true;
}

void SceneChangeStaging::BeginSceneEnd(StagingType stagingType, const Vector4& color)
{
	fade_->SetColor(color);
	spriteSceneChange_->SetColor(color);

	BeginSceneEnd(stagingType);
}

bool SceneChangeStaging::IsFinished()
{
	switch (stagingType_)
	{
	case StagingType::kFade:
		return fade_->IsFinished();
		break;
	case StagingType::kMixFade:
		return fade_->IsFinished() && fadeGray_->IsFinished();
		break;
	case StagingType::kEye:
		return spriteSceneChange_->GetIsEnd();
		break;
	default:
		break;
	}
	// デフォルトは終了とみなす
	return true;
}
