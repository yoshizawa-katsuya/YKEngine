#include "SelectScene.h"
#include "imgui/imgui.h"
#include "SceneManager.h"

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
	background_ = TextureManager::GetInstance()->Load("./resources/scene/background1.png");
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(background_);
	backgroundSprite_->SetPosition({ 0.0f,600.0f });

	//select
	std::vector<std::string> selectPaths = {
		"./resources/select/selectTutorial.png",
		"./resources/select/select1.png",
		"./resources/select/select2.png",
		"./resources/select/select3.png",
		"./resources/select/select4.png",
		"./resources/select/select5.png",
		"./resources/select/select6.png",
		"./resources/select/select7.png"
	};
	for (const auto& path : selectPaths) {
		selectTextures_.push_back(TextureManager::GetInstance()->Load(path));
	}
	std::vector<Vector2> selectPositions = {
		{232.0f, 569.0f}, {254.0f,400.0f}, {846.0f,472.0f}, {}, {}, {}, {}, {}
	};
	for (size_t i = 0; i < selectTextures_.size(); ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(selectTextures_[i]);
		if (i < selectPositions.size()) {
			sprite->SetPosition(selectPositions[i]);
		}
		selectSprites_.push_back(std::move(sprite));
	}
	
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
	std::vector<Vector2> bundlePositions = {
		{232.0f, 569.0f}, {254.0f,400.0f}, {846.0f,472.0f}, {}, {}, {}, {}, {}
	};
	for (size_t i = 0; i < bundleTextures_.size(); ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(bundleTextures_[i]);
		if (i < bundlePositions.size()) {
			sprite->SetPosition(bundlePositions[i]);
		}
		bundleSprites_.push_back(std::move(sprite));
	}

	//
	difficulty1_ = TextureManager::GetInstance()->Load("./resources/select/sam1.png");
	difficultySprite1_ = std::make_unique<Sprite>();
	difficultySprite1_->Initialize(difficulty1_);
	difficultySprite1_->SetPosition({ 0.0f,600.0f });
	difficulty2_ = TextureManager::GetInstance()->Load("./resources/select/sam2.png");
	difficultySprite2_ = std::make_unique<Sprite>();
	difficultySprite2_->Initialize(difficulty2_);
	difficultySprite2_->SetPosition({ 0.0f,600.0f });
	difficulty3_ = TextureManager::GetInstance()->Load("./resources/select/sam3.png");
	difficultySprite3_ = std::make_unique<Sprite>();
	difficultySprite3_->Initialize(difficulty3_);
	difficultySprite3_->SetPosition({ 0.0f,600.0f });
	


}

void SelectScene::Update()
{
	Vector2 mousePos = input_->GetMousePosition();
	static bool isClicked = false;
	if (input_->TrigerMouseLeft() && !isClicked) {
		isClicked = true;
		for (size_t i = 2; i < 8; i++)
		{
			if (IsPointInBounds(mousePos, { 846.0f, 472.0f }, { 990.0f, 735.0f })) {
				if (i > 1) {
					selectSprites_[i - 1]->SetPosition({ -342.0f,472.0f });
				}
				selectSprites_[i]->SetPosition({ 254.0f,400.0f });
				if (i + 1 < selectSprites_.size()) {
				selectSprites_[i + 1]->SetPosition({ 846.0f,472.0f });
				}
			break;

			}
		}
	}
	if (!input_->PushMouseLeft()) {
		isClicked = false;
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		//シーン切り替え依頼
		sceneManager_->ChengeScene("GameScene");
	}

#ifdef _DEBUG

	ImGui::Begin("Window");
	ImGui::Text("select");
	ImGui::DragFloat2("difficultySprite3_", &difficultySprite3_->GetSize().x, 0.11f);
	ImGui::DragFloat2("mouse", &mousePos.x, 0.11f);
	ImGui::Checkbox("isClicked", &isClicked);
	//ImGui::DragFloat2("selectdSprite1_", &selectdSprite1_->GetPosition().x, 0.11f);
	//ImGui::DragFloat2("selectdSprite2_", &selectdSprite2_->GetPosition().x, 0.11f);(
	ImGui::End();

#endif // _DEBUG


}

void SelectScene::Draw()
{

	//Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	spritePlatform_->PreDraw();

	backgroundSprite_->Draw();

	difficultySprite1_->Draw();
	

	if (input_->GetMousePosition().x < 144.0f || input_->GetMousePosition().x>836.0f) {
		difficultySprite3_->Draw();
	}
	else {
		difficultySprite2_->Draw();
	}

	selectSprites_[0]->Draw();
	selectSprites_[1]->Draw();
	selectSprites_[2]->Draw();
	selectSprites_[3]->Draw();
	selectSprites_[4]->Draw();
	selectSprites_[5]->Draw();
	selectSprites_[6]->Draw();
	selectSprites_[7]->Draw();

}

void SelectScene::Finalize()
{
}

bool SelectScene::IsPointInBounds(const Vector2& point, const Vector2& minBounds, const Vector2& maxBounds)
{
	return (point.x >= minBounds.x && point.x <= maxBounds.x &&
		point.y <= minBounds.y && point.y >= maxBounds.y);

}
