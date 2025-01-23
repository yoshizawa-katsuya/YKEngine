#include "SelectScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Vector.h"

SelectScene::~SelectScene()
{
}

void SelectScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();


	//background
	background_ = TextureManager::GetInstance()->Load("./resources/select/background2.png");
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(background_);
	backgroundSprite_->SetPosition({ 0.0f,0.0f });

	//select
	selectTutorial_ = TextureManager::GetInstance()->Load("./resources/select/selectTutorial.png");
	tutorialSprite_ = std::make_unique<Sprite>();
	tutorialSprite_->Initialize(selectTutorial_);
	tutorialSprite_->SetPosition({ 232.0f, 31.0f });

	selectTextureHandles_[0] = TextureManager::GetInstance()->Load("./resources/select/select1.png");
	selectTextureHandles_[1] = TextureManager::GetInstance()->Load("./resources/select/select2.png");
	selectTextureHandles_[2] = TextureManager::GetInstance()->Load("./resources/select/select3.png");
	selectTextureHandles_[3] = TextureManager::GetInstance()->Load("./resources/select/select4.png");
	selectTextureHandles_[4] = TextureManager::GetInstance()->Load("./resources/select/select5.png");
	selectTextureHandles_[5] = TextureManager::GetInstance()->Load("./resources/select/select6.png");
	selectTextureHandles_[6] = TextureManager::GetInstance()->Load("./resources/select/select7.png");

	selectSprites_[static_cast<uint32_t>(positionType::kLeftSide)] = std::make_unique<Sprite>();
	selectSprites_[static_cast<uint32_t>(positionType::kLeftSide)]->Initialize(selectTextureHandles_[6]);
	selectSprites_[static_cast<uint32_t>(positionType::kLeftSide)]->SetPosition({ -342.0f,128.0f });

	selectSprites_[static_cast<uint32_t>(positionType::kCenter)] = std::make_unique<Sprite>();
	selectSprites_[static_cast<uint32_t>(positionType::kCenter)]->Initialize(selectTextureHandles_[0]);
	selectSprites_[static_cast<uint32_t>(positionType::kCenter)]->SetPosition({ 254.0f,200.0f });

	selectSprites_[static_cast<uint32_t>(positionType::kRightSide)] = std::make_unique<Sprite>();
	selectSprites_[static_cast<uint32_t>(positionType::kRightSide)]->Initialize(selectTextureHandles_[1]);
	selectSprites_[static_cast<uint32_t>(positionType::kRightSide)]->SetPosition({ 846.0f,128.0f });

	
	//bundle
	std::vector<std::string> bundlePaths = {
		"./resources/select/bundle/bundle1.png",
		"./resources/select/bundle/bundle2.png",
		"./resources/select/bundle/bundle3.png",
		"./resources/select/bundle/bundle4.png",
		"./resources/select/bundle/bundle5.png",
		"./resources/select/bundle/bundle6.png",
		"./resources/select/bundle/bundle7.png"
	};
	for (const auto& path : bundlePaths) {
		bundleTextures_.push_back(TextureManager::GetInstance()->Load(path));
	}
	std::vector<Vector2> bundlePositions(7, Vector2(275.0f, 50.0f));
	for (size_t i = 0; i < bundleTextures_.size(); ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(bundleTextures_[i]);
		if (i < bundlePositions.size()) {
			sprite->SetPosition(bundlePositions[i]);
		}
		bundleSprites_.push_back(std::move(sprite));
	}
	//select stage
	std::vector<std::string> stagePaths = {
		"./resources/select/selectStage/stage1.png",
		"./resources/select/selectStage/stage2.png",
		"./resources/select/selectStage/stage3.png",
		"./resources/select/selectStage/stage4.png",
		"./resources/select/selectStage/stage5.png",
		"./resources/select/selectStage/stage6.png",
		"./resources/select/selectStage/stage7.png",
		"./resources/select/selectStage/stage8.png",
		"./resources/select/selectStage/stage9.png",
		"./resources/select/selectStage/stage10.png"
	};
	for (const auto& path : stagePaths) {
		stageTextures_.push_back(TextureManager::GetInstance()->Load(path));
	}
	std::vector<Vector2> stagePositions = {
		{65.0f, 220.0f}, {245.0f,220.0f}, {425.0f,220.0f}, {605.0f,220.0f}, {785.0f,220.0f},
		{65.0f, 370.0f}, {245.0f,370.0f}, {425.0f,370.0f}, {605.0f,370.0f}, {785.0f,370.0f}
	};
	for (size_t i = 0; i < stageTextures_.size(); ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(stageTextures_[i]);
		if (i < stageTextures_.size()) {
			sprite->SetPosition(stagePositions[i]);
		}
		stageSprites_.push_back(std::move(sprite));
	}



	//
	std::vector<std::string> difficultyPaths = {
		"./resources/select/sam1.png",
		"./resources/select/sam2.png",
		"./resources/select/sam3.png"
	};
	for (const auto& path : difficultyPaths) {
		difficulty_.push_back(TextureManager::GetInstance()->Load(path));
	}
	std::vector<Vector2> difficultyPositions = {
		{0.0f,0.0f}, {0.0f,0.0f}, {0.0f,0.0f}
	};
	for (size_t i = 0; i < difficulty_.size(); ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(difficulty_[i]);
		if (i < difficultyPositions.size()) {
			sprite->SetPosition(difficultyPositions[i]);
		}
		difficultySprites_.push_back(std::move(sprite));
	}
	

}

void SelectScene::Update()
{
	
	if (input_->TrigerMouseLeft()) {
		Square square;
		square.min = selectSprites_[static_cast<uint32_t>(positionType::kRightSide)]->GetPosition();
		square.max = square.min + selectSprites_[static_cast<uint32_t>(positionType::kRightSide)]->GetSize();
		if (IsCollision(square, input_->GetMousePosition())) {
			selectStageNum_++;
			if (selectStageNum_ > kMaxStageNum_) {
				selectStageNum_ = 1;
			}
			uint32_t leftSelectStageNum = selectStageNum_ - 1;
			uint32_t rightSelectStageNum = selectStageNum_ + 1;
			if (leftSelectStageNum < 1) {
				leftSelectStageNum = kMaxStageNum_;
			}
			if (rightSelectStageNum > kMaxStageNum_) {
				rightSelectStageNum = 1;
			}

			selectSprites_[static_cast<uint32_t>(positionType::kLeftSide)]->SetTexture(selectTextureHandles_[leftSelectStageNum - 1]);
			selectSprites_[static_cast<uint32_t>(positionType::kCenter)]->SetTexture(selectTextureHandles_[selectStageNum_ - 1]);
			selectSprites_[static_cast<uint32_t>(positionType::kRightSide)]->SetTexture(selectTextureHandles_[rightSelectStageNum - 1]);

		} else {
			square.min = selectSprites_[static_cast<uint32_t>(positionType::kLeftSide)]->GetPosition();
			square.max = square.min + selectSprites_[static_cast<uint32_t>(positionType::kLeftSide)]->GetSize();
			if (IsCollision(square, input_->GetMousePosition())) {
				selectStageNum_--;
				if (selectStageNum_ < 1) {
					selectStageNum_ = kMaxStageNum_;
				}
				uint32_t leftSelectStageNum = selectStageNum_ - 1;
				uint32_t rightSelectStageNum = selectStageNum_ + 1;
				if (leftSelectStageNum < 1) {
					leftSelectStageNum = kMaxStageNum_;
				}
				if (rightSelectStageNum > kMaxStageNum_) {
					rightSelectStageNum = 1;
				}

				selectSprites_[static_cast<uint32_t>(positionType::kLeftSide)]->SetTexture(selectTextureHandles_[leftSelectStageNum - 1]);
				selectSprites_[static_cast<uint32_t>(positionType::kCenter)]->SetTexture(selectTextureHandles_[selectStageNum_ - 1]);
				selectSprites_[static_cast<uint32_t>(positionType::kRightSide)]->SetTexture(selectTextureHandles_[rightSelectStageNum - 1]);

			}
		}

	}

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}


	ImGui::Begin("Window");
	ImGui::Text("SelectScene");
	for (size_t i = 0; i < 10; i++)
	{
		std::string label = "stageSprites_[" + std::to_string(i) + "]";
		ImGui::DragFloat2(label.c_str(), &stageSprites_[i]->GetPosition().x, 0.11f);
	}
	ImGui::DragFloat2("size", &stageSprites_[0]->GetSize().x, 0.11f);
	ImGui::DragFloat2("Tsize", &stageSprites_[0]->GetTextureSize().x, 0.11f);
	/*
	ImGui::DragFloat2("selectdSprite1_", &selectdSprite1_->GetPosition().x, 0.11f);
	ImGui::DragFloat2("selectdSprite2_", &selectdSprite2_->GetPosition().x, 0.11f);
	*/
	ImGui::End();

}

void SelectScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	backgroundSprite_->Draw();

	tutorialSprite_->Draw();

	


	if (input_->GetMousePosition().x < 144.0f || input_->GetMousePosition().x>836.0f) {
		difficultySprites_[0]->Draw();
		difficultySprites_[2]->Draw();
	} else {
		difficultySprites_[0]->Draw();
		difficultySprites_[1]->Draw();
	}
	if (selectSpr) {
	for (std::unique_ptr<Sprite>& selectSprite : selectSprites_) {
		selectSprite->Draw();
	}
	}

	
	//if (input_->PushMouseLeft() && IsMouseOverSprite(input_->GetMousePosition(), selectSprites_[1])) {
	//	// selectSprites_[1]의 텍스처 핸들을 가져옴
	//	uint32_t currentTexture = selectSprites_[1]->get
	//
	//	// selectTextureHandles_와 비교
	//	for (size_t i = 0; i < selectTextureHandles_.size(); ++i) {
	//		if (currentTexture == selectTextureHandles_[i]) {
	//			// 대응되는 bundleSprites_ 드로우
	//			bundleSprites_[i]->Draw();
	//			break; // 일치하는 텍스처를 찾으면 반복문 종료
	//		}
	//	}
	//}
	
	




	//for (std::unique_ptr<Sprite>& selectBund : bundleSprites_) {
	//	selectBund->Draw();
	//}
	//for (std::unique_ptr<Sprite>& stageSprite : stageSprites_) {
	//	stageSprite->Draw();
	//}
}

void SelectScene::Finalize()
{
}

bool SelectScene::IsMouseOverSprite(const Vector2& mousePos, const std::unique_ptr<Sprite>& sprite)
{
	if (!sprite) return false;

	
	Vector2 spritePos = sprite->GetPosition();
	Vector2 spriteSize = sprite->GetSize();

	float left = spritePos.x;
	float right = spritePos.x + spriteSize.x;
	float bottom = spritePos.y;
	float top = spritePos.y + spriteSize.y;

	return (mousePos.x >= left && mousePos.x <= right &&
		mousePos.y >= bottom && mousePos.y <= top);
}
