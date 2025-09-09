#include "PauseMenu.h"
#include "SceneManager.h"
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
			"Resources/pause/retry.png",//リトライ
			"Resources/pause/control.png",//操作方法へ
			"Resources/pause/backselect.png",//セレクトへ
			"Resources/pause/backtitle.png",//タイトルへ
			"Resources/pause/controltile.png",//操作方法画面
	};

	positions = {
		Vector2{650.0f,355.0f},//背景
		Vector2{650.0f,70.0f},//リトライ
		Vector2{650.0f,220.0f},//操作方法
		Vector2{650.0f,400.0f},//セレクトへ
		Vector2{650.0f,580.0f},//タイトル
		Vector2{650.0f,350.0f},//操作方法画面
	};

	for (size_t i = 0; i < sprites_.size(); i++) {
		sprites_[i] = std::make_unique<Sprite>();
		sprites_[i]->Initialize(TextureManager::GetInstance()->Load(texturePaths[i]));
		sprites_[i]->SetAnchorPoint({ 0.5f, 0.5f });
		sprites_[i]->SetPosition(positions[i]);
		sizes[i] = sprites_[i]->GetTextureSize();
	}
	sizes[0] = Vector2{ 500.0f, 700.0f };//背景
	sizes[1] = Vector2{ 490.0f, 204.0f };//リトライ
	sizes[2] = Vector2{ 490.0f, 204.0f };//操作方法
	sizes[3] = Vector2{ 490.0f, 204.0f };//セレクトへ
	sizes[4] = Vector2{ 490.0f,204.0f };//タイトルへ
	sizes[5] = Vector2{ 950.0f,630.0f };//操作方法画面

	for (size_t i = 0; i < sprites_.size(); i++) {
		sprites_[i]->SetSize(sizes[i]);
	}

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	menuState = 1;
}
void PauseMenu::Update() {
	//Tキーで出し入れ
	if (!isControlScreen_&&Input::GetInstance()->TriggerKey(DIK_T)) {
		isPaused_ = !isPaused_;
	}

	if (isPaused_) {
		//イージングタイマー
		easeTimer_ += easeSpeed;
		if (easeTimer_ > 1.0f)easeTimer_ = 1.0f;
		//メニュー内移動
		if (!fadeStart_) {
			if (Input::GetInstance()->TriggerKey(DIK_W)) {
				menuState--;
				if (menuState < 1) menuState = 1;
			} else if (Input::GetInstance()->TriggerKey(DIK_S)) {
				menuState++;
				if (menuState > 4) menuState = 4;
			}
		}

		UpdateMenu();
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

	//操作方法画面イージング
	if (isControlScreen_) {
		controlEaseTimer_ += easeSpeed;
		if (controlEaseTimer_ > 1.0f) controlEaseTimer_ = 1.0f;
	} else {
		controlEaseTimer_ -= easeSpeed;
		if (controlEaseTimer_ < 0.0f) controlEaseTimer_ = 0.0f;
	}

	float controlScale = static_cast<float>(easeInOutCirc(controlEaseTimer_));
	sprites_[5]->SetPosition(positions[5]);
	sprites_[5]->SetSize({ sizes[5].x * controlScale, sizes[5].y * controlScale });

	//操作方法画面から戻る
	if (isControlScreen_ && Input::GetInstance()->TriggerKey(DIK_F)) {
		isControlScreen_ = false;
	}

	//メニュー内選択演出
	if (isPaused_) {
		cursorTimer_ += cursorSpeed_ * 1.0f / 60.0f;
		float animScale = 1.0f + std::sin(cursorTimer_) * cursorAmplitude_;

		for (size_t i = 1; i <= 4; i++) {
			if (i == menuState) {
				sprites_[i]->SetSize({ sizes[i].x * easedValue * animScale,
									   sizes[i].y * easedValue * animScale });
			} else {
				sprites_[i]->SetSize({ sizes[i].x * easedValue, sizes[i].y * easedValue });
			}
		}
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

	fade_->Update();
}
void PauseMenu::Draw() {
	if (easeTimer_ > 0.0f) {
		if (!isControlScreen_) {
			for (size_t i = 0; i < sprites_.size() - 1; i++) {
				sprites_[i]->Draw();
			}
		} else {
			//操作方法画面
			sprites_[5]->Draw();
		}
	}
	//フェード描画
	fade_->Draw();

}
void PauseMenu::UpdateMenu() {

	//操作方法画面
	if (isControlScreen_) {
		if (Input::GetInstance()->TriggerKey(DIK_F)) {
			isControlScreen_ = false;
		}
		return;
	}

	//フェード
	if (fadeStart_) {
		if (fade_->IsFinished()) {
			fadeStart_ = false;
			SceneManager::GetInstance()->ChengeScene(nextScene_);
		}
		return;
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		if (menuState == 1) {//リトライ
			nextScene_ = "GameScene";
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		} else if (menuState == 2) {//操作説明画面
			isControlScreen_ = true;
		} else if (menuState == 3) { //セレクト
			fadeStart_ = true;
			nextScene_ = "StageSelectScene";
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		} else if (menuState == 4) { //タイトル
			fadeStart_ = true;
			nextScene_ = "TitleScene";
			fade_->Start(Fade::Status::FadeOut, 0.5f);
		}
	}
}
//ポーズ中かを取得
bool PauseMenu::IsPaused() {
	return isPaused_ || easeTimer_ > 0.0f;
}
