#include "SelectStage.h"
#include "imgui/imgui.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Vector.h"
#include <algorithm>

SelectStage::~SelectStage()
{
}

void SelectStage::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();


	//background
	//background_ = TextureManager::GetInstance()->Load("./resources/select/background3.png");
	background_ = TextureManager::GetInstance()->Load("./resources/select/titleBack2.png");
	backgroundSprite_ = std::make_unique<Sprite>();
	backgroundSprite_->Initialize(background_);
	backgroundSprite_->SetPosition({ 0.0f,0.0f });

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

	//return
	std::vector<std::string> reButtonPaths = {
		"./resources/select/return.png",
		"./resources/select/return2.png",
		"./resources/select/return3.png"
	};
	//return
	reButton1_ = TextureManager::GetInstance()->Load("./resources/select/return.png");
	reButtonSprite1_ = std::make_unique<Sprite>();
	reButtonSprite1_->Initialize(reButton1_);
	reButtonSprite1_->SetPosition({ 878.0f, 513.0f });

	reButton2_ = TextureManager::GetInstance()->Load("./resources/select/return2.png");
	reButtonSprite2_ = std::make_unique<Sprite>();
	reButtonSprite2_->Initialize(reButton2_);
	reButtonSprite2_->SetPosition({ 860.0f, 495.0f });

	reButton3_ = TextureManager::GetInstance()->Load("./resources/select/return3.png");
	reButtonSprite3_ = std::make_unique<Sprite>();
	reButtonSprite3_->Initialize(reButton3_);
	reButtonSprite3_->SetPosition({ 875.0f, 463.0f });
	

	SceneData data = SceneManager::GetInstance()->GetSceneData();

	selectedBundle_ = data.selectedBundle;

	set = false;
}

void SelectStage::Update()
{

		Square reSize;
		reSize.min = { reButtonSprite1_.get()->GetPosition() };
		reSize.max = { reButtonSprite1_.get()->GetPosition() + reButtonSprite1_.get()->GetSize() };

	bool reButtonInside = IsCollision(reSize, input_->GetMousePosition());

	if (reButtonInside) {
		reButtonSprite3_->SetVisible(true);
		reButtonSprite2_->SetVisible(true);
		reButtonSprite1_->SetVisible(false);
	} else {
		reButtonSprite3_->SetVisible(false);
		reButtonSprite2_->SetVisible(false);
		reButtonSprite1_->SetVisible(true);
	}
	if (reButtonInside && input_->TrigerMouseLeft()) {
		sceneManager_->ChengeScene("SelectScene");
	}
	for (size_t i = 0; i < 10; ++i) {
		if (input_->PushMouseLeft()&&IsMouseOverSprite(input_->GetMousePosition(), stageSprites_[i])) {
			set = true;
			selectedStage_ = static_cast<uint32_t>(i) + 1;
			SceneManager::GetInstance()->SetSelectedStage(selectedStage_);
			sceneManager_->ChengeScene("GameScene");
		}
		
		
	}

#ifdef _DEBUG
	ImGui::Begin("SelectStage");

	ImGui::Text("Selected Bundle: %u", selectedBundle_);
	ImGui::Text("set: %u", set);
	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);
	ImGui::End();
#endif // _DEBUG

}

void SelectStage::Draw()
{

	spritePlatform_->PreDraw();

	backgroundSprite_->Draw();


	int drawCount = static_cast<int>(selectedBundle_) - 1;
	bundleSprites_[drawCount]->Draw();

	for (std::unique_ptr<Sprite>& stageSprite : stageSprites_) {
		stageSprite->Draw();
	}
	
	if (reButtonSprite1_->IsVisible()) {
		reButtonSprite1_->Draw();
	}

	if (reButtonSprite2_->IsVisible()) {
		reButtonSprite2_->Draw();
	}
	if (reButtonSprite3_->IsVisible()) {
		reButtonSprite3_->Draw();
	}

}

void SelectStage::Finalize()
{
}

bool SelectStage::IsMouseOverSprite(const Vector2& mousePos, const std::unique_ptr<Sprite>& sprite)
{
	if (!sprite) return false;


	Vector2 spritePos = sprite->GetPosition();
	Vector2 spriteSize = { 140.0f,96.0f };

	float left = spritePos.x;
	float right = spritePos.x + spriteSize.x;
	float bottom = spritePos.y;
	float top = spritePos.y + spriteSize.y;

	return (mousePos.x >= left && mousePos.x <= right &&
		mousePos.y >= bottom && mousePos.y <= top);
}

uint32_t SelectStage::GetSelectedStage() const
{
	return selectedStage_;
}
