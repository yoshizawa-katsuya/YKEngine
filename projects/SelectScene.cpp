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

}

void SelectScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	tutorialSprite_->Draw();

	for (std::unique_ptr<Sprite>& selectSprite : selectSprites_) {
		selectSprite->Draw();
	}
	

}

void SelectScene::Finalize()
{
}
