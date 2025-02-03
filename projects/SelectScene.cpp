#include "SelectScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Vector.h"
#include <algorithm>

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
	background_ = TextureManager::GetInstance()->Load("./resources/select/background3.png");
	background_ = TextureManager::GetInstance()->Load("./resources/select/titleBack2.png");
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

	
	
	
	//
	std::vector<std::string> difficultyPaths = {
		"./resources/select/sam1.png",
		"./resources/select/sam2.png",
		"./resources/select/sam3.png",
		"./resources/select/sam4.png",
		"./resources/select/sam5.png"
	};
	for (const auto& path : difficultyPaths) {
		difficulty_.push_back(TextureManager::GetInstance()->Load(path));
	}
	std::vector<Vector2> difficultyPositions = {
		{0.0f,0.0f}, {0.0f,0.0f}, {0.0f,0.0f}, {0.0f,0.0f}, {0.0f,0.0f}
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
		Vector2 mousePos = input_->GetMousePosition();
	if (selectClick1) {
		if (input_->PushMouseLeft() && IsMouseOverSprite(mousePos, tutorialSprite_)) {
			selectedTutorial_ = 1;
			SceneManager::GetInstance()->SetSelectedTutorial(selectedTutorial_);
			sceneManager_->ChengeScene("GameScene");
		}
	}

	if (selectClick1) {
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
		Square difficultySizeL;
		difficultySizeL.min = { 0.0f,128.0f };
		difficultySizeL.max = { 144.0f,391.0f };

		Square difficultySizeR;
		difficultySizeR.min = { 846.0f,128.0f };
		difficultySizeR.max = { 990.0f,391.0f };

		bool isMouseInsideL = IsCollision(difficultySizeL, input_->GetMousePosition());
		bool isMouseInsideR = IsCollision(difficultySizeR, input_->GetMousePosition());

		if (isMouseInsideL) {
			difficultySprites_[2]->SetVisible(true);
			difficultySprites_[1]->SetVisible(false);
		} else {
			difficultySprites_[2]->SetVisible(false);
			difficultySprites_[1]->SetVisible(true);
		}

		if (isMouseInsideR) {
			difficultySprites_[4]->SetVisible(true);
			difficultySprites_[3]->SetVisible(false);
		} else {
			difficultySprites_[4]->SetVisible(false);
			difficultySprites_[3]->SetVisible(true);
		}
		if (input_->PushMouseLeft() && IsMouseOverSprite(input_->GetMousePosition(), selectSprites_[1])) {

			uint32_t currentTexture = selectSprites_[1]->GetTeture();
			for (size_t i = 0; i < selectTextureHandles_.size(); ++i) {
				if (currentTexture == selectTextureHandles_[i]) {
					selectedBundle_ = static_cast<uint32_t>(i) + 1;
					SceneManager::GetInstance()->SetSelectedBundle(selectedBundle_);
					break;
				}
			}

			selectClick1 = false;
		}
	}

	if (!selectClick1) {
		sceneManager_->ChengeScene("SelectStage");
	}
	


#ifdef _DEBUG
	ImGui::Begin("Window");
	ImGui::Text("SelectScene");
	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);
	ImGui::Text("selectClick1: %u", selectClick1);
	ImGui::Text("selectClick2: %u", selectClick2);
	ImGui::Text("PushMouseLeft: %u", input_->PushMouseLeft());
	/*
	ImGui::DragFloat2("selectdSprite1_", &selectdSprite1_->GetPosition().x, 0.11f);
	ImGui::DragFloat2("selectdSprite2_", &selectdSprite2_->GetPosition().x, 0.11f);
	*/
	ImGui::End();
#endif // _DEBUG
}

void SelectScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	backgroundSprite_->Draw();

	difficultySprites_[0]->Draw();


	
	tutorialSprite_->Draw();

	if (selectClick1) {
		for (std::unique_ptr<Sprite>& selectSprite : selectSprites_) {
			selectSprite->Draw();
		}
	}

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

uint32_t SelectScene::GetSelectedTutorial() const
{
	return selectedTutorial_;
}

uint32_t SelectScene::GetSelectedBundle() const
{
	return selectedBundle_;
}


