#include "SceneChangeStaging.h"
#include "TextureManager.h"
#include "Fade.h"

using namespace YKEngine;

SceneChangeStaging* SceneChangeStaging::instance_ = nullptr;

SceneChangeStaging* SceneChangeStaging::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneChangeStaging();
	}
	return instance_;
}

void SceneChangeStaging::Finalize()
{
	//インスタンスを破棄
	delete instance_;
	instance_ = nullptr;
}

void SceneChangeStaging::Initialize()
{
	uint32_t textureHandleSceneChange = TextureManager::GetInstance()->Load("./Resources/SceneChange01_sheet.png");

	//スプライトの生成
	spriteSceneChange_ = std::make_unique<AnimatedSprite>();
	spriteSceneChange_->Initialize(textureHandleSceneChange, 20, 3);
	spriteSceneChange_->SetSize({ WinApp::kClientWidth , WinApp::kClientHeight });
	spriteSceneChange_->SetIsLoop(false);

	//フェードの初期化
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
}

void SceneChangeStaging::Update()
{
	if (status_ == Status::kNone)
	{
		return;
	}

	switch (stagingType_)
	{
	case StagingType::kFade:
		fade_->Update();
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
	case StagingType::kEye:
		spriteSceneChange_->Reset();
		break;
	default:
		break;
	}
	status_ = Status::kStart;
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
	case StagingType::kEye:
		spriteSceneChange_->ResetReverseAnimation();
		break;
	default:
		break;
	}

	status_ = Status::kEnd;
}

bool SceneChangeStaging::IsFinished()
{
	switch (stagingType_)
	{
	case StagingType::kFade:
		return fade_->IsFinished();
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
