#include "TitleScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Vector.h"

TitleScene::~TitleScene()
{
	//Finalize();
}

void TitleScene::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();
	/*
	textureHandle_ = TextureManager::GetInstance()->Load("./resources/Title.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(textureHandle_, spritePlatform_);
	sprite_->SetPosition({ 100.0f, 100.0f });
	*/

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

	/*
	select1_ = TextureManager::GetInstance()->Load("./resources/select/select1.png");
	selectdSprite1_ = std::make_unique<Sprite>();
	selectdSprite1_->Initialize(select1_);
	selectdSprite1_->SetPosition({ 254.0f,200.0f });
	select2_ = TextureManager::GetInstance()->Load("./resources/select/select2.png");
	selectdSprite2_ = std::make_unique<Sprite>();
	selectdSprite2_->Initialize(select2_);
	selectdSprite2_->SetPosition({ 846.0f,128.0f });
	select3_ = TextureManager::GetInstance()->Load("./resources/select/select3.png");
	selectdSprite3_ = std::make_unique<Sprite>();
	selectdSprite3_->Initialize(select3_);
	select4_ = TextureManager::GetInstance()->Load("./resources/select/select4.png");
	selectdSprite4_ = std::make_unique<Sprite>();
	selectdSprite4_->Initialize(select4_);
	select5_ = TextureManager::GetInstance()->Load("./resources/select/select5.png");
	selectdSprite5_ = std::make_unique<Sprite>();
	selectdSprite5_->Initialize(select5_);
	select6_ = TextureManager::GetInstance()->Load("./resources/select/select6.png");
	selectdSprite6_ = std::make_unique<Sprite>();
	selectdSprite6_->Initialize(select6_);
	select7_ = TextureManager::GetInstance()->Load("./resources/select/select7.png");
	selectdSprite7_ = std::make_unique<Sprite>();
	selectdSprite7_->Initialize(select7_);
	*/

}

void TitleScene::Update()
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

		}
		else {
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
	


#ifdef _DEBUG
	
	ImGui::Begin("Window");
	ImGui::Text("Title");
	ImGui::DragFloat2("tutorialSprite_", &tutorialSprite_->GetPosition().x, 0.11f);
	/*
	ImGui::DragFloat2("selectdSprite1_", &selectdSprite1_->GetPosition().x, 0.11f);
	ImGui::DragFloat2("selectdSprite2_", &selectdSprite2_->GetPosition().x, 0.11f);
	*/
	ImGui::End();
	
#endif // _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}

}

void TitleScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();


	tutorialSprite_->Draw();

	for (std::unique_ptr<Sprite>& selectSprite : selectSprites_) {
		selectSprite->Draw();
	}

	//selectdSprite1_->Draw();
	//selectdSprite2_->Draw();
	//selectdSprite3_->Draw();
	//selectdSprite4_->Draw();
	//selectdSprite5_->Draw();
	//selectdSprite6_->Draw();
	//selectdSprite7_->Draw();

}

void TitleScene::Finalize()
{

}
