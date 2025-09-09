#include "PauseMenu.h"
#include "Input.h"
#include "imgui/imgui.h"

double PauseMenu::easeInOutCirc(double x) {
	return (x < 0.5)
		? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
		: (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
}

void PauseMenu::Initialize() {
	spritePlatform_ = SpritePlatform::GetInstance();
	modelPlatform_ = ModelPlatform::GetInstance();

	texturePaths = {
			"Resources/pause/background.png",//背景
			"Resources/pause/control.png",//操作方法
			"Resources/pause/backselect.png",//セレクトへ
			"Resources/pause/backtitle.png",//タイトルへ
	};

	positions = {
		Vector2{650.0f,355.0f},//背景
		Vector2{650.0f,85.0f},//操作方法
		Vector2{650.0f,280.0f},//セレクトへ
		Vector2{650.0f,502.0f},//タイトル
	};

	for (size_t i = 0; i < sprites_.size(); i++) {
		sprites_[i] = std::make_unique<Sprite>();
		sprites_[i]->Initialize(TextureManager::GetInstance()->Load(texturePaths[i]));
		sprites_[i]->SetAnchorPoint({ 0.5f, 0.5f });
		sprites_[i]->SetPosition(positions[i]);
		sizes[i] = sprites_[i]->GetTextureSize();
	}
	sizes[0] = Vector2{ 500.0f, 700.0f };
	sizes[1] = Vector2{ 490.0f, 204.0f };
	sizes[2] = Vector2{ 490.0f, 204.0f };
	sizes[3] = Vector2{ 490.0f, 204.0f };

	for (size_t i = 0; i < sprites_.size(); i++) {
		sprites_[i]->SetSize(sizes[i]);
	}
}
void PauseMenu::Update() {
	//Tキーで出し入れ
	if (Input::GetInstance()->TriggerKey(DIK_T)) {
		isPaused_ = !isPaused_;
	}

	if (isPaused_) {
		easeTimer_ += easeSpeed;
		if (easeTimer_ > 1.0f)easeTimer_ = 1.0f;
	} else {
		easeTimer_ -= easeSpeed;
		if (easeTimer_ < 0.0f)easeTimer_ = 0.0f;
	}
	//イージング値
	float easedValue = static_cast<float>(easeInOutCirc(easeTimer_));

	for (size_t i = 0; i < sprites_.size(); i++) {
		float scale = (i == 0) ? easedValue : easedValue;
		Vector2 baseSize = sprites_[i]->GetTextureSize();
		sprites_[i]->SetPosition(positions[i]);
		sprites_[i]->SetSize({ sizes[i].x * easedValue, sizes[i].y * easedValue });
	}
#ifdef _DEBUG
	if (ImGui::Begin("PauseMenu Debug")) {
		for (size_t i = 0; i < sprites_.size(); i++) {
			ImGui::PushID(static_cast<int>(i));
			ImGui::Text("Sprite %zu", i);
			ImGui::DragFloat2("Pos", &positions[i].x, 1.0f); 
			ImGui::DragFloat2("Size", &sizes[i].x, 1.0f); 
			ImGui::Separator();
			ImGui::PopID();
		}
	}
	ImGui::End();
#endif
}
void PauseMenu::Draw() {
	if (easeTimer_ > 0.0f) {
		for (size_t i = 0; i < sprites_.size(); i++) {
			sprites_[i]->Draw();
		}
	}
}