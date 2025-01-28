#include "GameScene.h"
#include "dx12.h"
#include "imgui/imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "RigidModel.h"
#include "Vector.h"

GameScene::~GameScene() {
    // Finalize();
}

void GameScene::Initialize() {
    dxCommon_ = DirectXCommon::GetInstance();
    audio_ = Audio::GetInstance();
    input_ = Input::GetInstance();
    spritePlatform_ = SpritePlatform::GetInstance();
    modelPlatform_ = ModelPlatform::GetInstance();

    // 平行光源の生成
    directionalLight_ = std::make_unique<DirectionalLight>();
    directionalLight_->Initialize(dxCommon_);

    // 点光源の生成
    pointLight_ = std::make_unique<PointLight>();
    pointLight_->Initialize(dxCommon_);

    // スポットライトの生成
    spotLight_ = std::make_unique<SpotLight>();
    spotLight_->Initialize(dxCommon_);

    // カメラの生成
    camera_ = std::make_unique<Camera>();
    camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
    camera_->SetTranslate({ 0.0f, 0.0f, -10.0f });

    // デバッグカメラの生成
    camera2_ = std::make_unique<Camera>();
    camera2_->SetRotate({ 0.0f, 0.0f, 0.0f });
    camera2_->SetTranslate({ 0.0f, 0.0f, -10.0f });
    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize(camera2_.get(), input_);

    // メインカメラの設定
    mainCamera_ = camera_.get();

    // モデルを描画する際ライトとカメラの設定は必須
    modelPlatform_->SetDirectionalLight(directionalLight_.get());
    modelPlatform_->SetPointLight(pointLight_.get());
    modelPlatform_->SetCamera(mainCamera_);
    modelPlatform_->SetSpotLight(spotLight_.get());

    textureHandle_ = TextureManager::GetInstance()->Load("./resources/circle.png");

    // モデルの生成
    modelPlayer_ = std::make_unique<RigidModel>();
    modelPlayer_->CreateModel("./resources/Player", "Player.obj");

    // ストーンモデルの初期化
    modelStone_ = std::make_unique<RigidModel>();
    modelStone_->CreateModel("./resources", "plane.obj");

    // プレイヤーの初期化
    player_ = std::make_unique<Player>();
    player_->Initialize(modelPlayer_.get());

    // 初期のストーンを追加
    auto stone = std::make_unique<Stone>();
    stone->Initialize(modelStone_.get());
    stones_.push_back(std::move(stone));
}

void GameScene::Update() {
  
    for (size_t i = 0; i < stones_.size(); ++i) {
        if (i == stones_.size() - 1) {
            stones_[i]->Update();  // 現在のストーンのみ操作
        }
        else if (stones_[i]->GetState() == Stone::State::Flying) {
            stones_[i]->Update();
        }
    }

    // 衝突判定
    for (size_t i = 0; i < stones_.size(); ++i) {
        for (size_t j = i + 1; j < stones_.size(); ++j) {
            if (stones_[i]->CheckCollision(*stones_[i], *stones_[j])) {
               stones_[i]->HandleCollision(*stones_[j]);
            }
        }
    }

    // 最後のストーンがStoppedになったら新しいストーンを追加
    if (!stones_.empty() && stones_.back()->GetState() == Stone::State::Stopped) {
        auto newStone = std::make_unique<Stone>();
        newStone->Initialize(modelStone_.get());
        stones_.push_back(std::move(newStone));
    }

    // カメラの更新
    camera_->Update();

    if (isActiveDebugCamera_) {
        debugCamera_->Update();
    }
    
#ifdef _DEBUG
    ImGui::Begin("Window");
    if (ImGui::TreeNode("camera")) {
        ImGui::DragFloat3("translate", &camera_->GetTranslate().x, 0.01f);
        ImGui::DragFloat3("rotate", &camera_->GetRotate().x, 0.01f);
        //ImGui::DragFloat3("scale", &cameratransform.scale.x, 0.01f);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("DirectionalLight")) {
        ImGui::ColorEdit4("color", &directionalLight_->GetColor().x);
        ImGui::DragFloat3("direction", &directionalLight_->GetDirection().x, 0.01f);
        ImGui::DragFloat("intensity", &directionalLight_->GetIntensity(), 0.01f);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("PointLight")) {
        ImGui::ColorEdit4("color", &pointLight_->GetColor().x);
        ImGui::DragFloat3("position", &pointLight_->GetPosition().x, 0.01f);
        ImGui::DragFloat("intensity", &pointLight_->GetIntensity(), 0.01f);
        ImGui::DragFloat("radius", &pointLight_->GetRadius(), 0.01f);
        ImGui::DragFloat("decay", &pointLight_->GetDecay(), 0.01f);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("SpotLight")) {
        ImGui::ColorEdit4("color", &spotLight_->GetColor().x);
        ImGui::DragFloat3("position", &spotLight_->GetPosition().x, 0.01f);
        ImGui::DragFloat("intensity", &spotLight_->GetIntensity(), 0.01f);
        ImGui::DragFloat3("direction", &spotLight_->GetDirection().x, 0.01f);
        ImGui::DragFloat("distance", &spotLight_->GetDistance(), 0.01f);
        ImGui::DragFloat("decay", &spotLight_->GetDecay(), 0.01f);
        ImGui::DragFloat("cosAngle", &spotLight_->GetCosAngle(), 0.01f);
        ImGui::DragFloat("cosFalloffStart", &spotLight_->GetCosFalloffStart(), 0.01f);

        ImGui::TreePop();
    }
    //メインカメラの切り替え
    if (ImGui::RadioButton("gameCamera", !isActiveDebugCamera_)) {
        isActiveDebugCamera_ = false;

        mainCamera_ = camera_.get();
        modelPlatform_->SetCamera(mainCamera_);

    }
    if (ImGui::RadioButton("DebugCamera", isActiveDebugCamera_)) {
        isActiveDebugCamera_ = true;

        mainCamera_ = camera2_.get();
        modelPlatform_->SetCamera(mainCamera_);

	}
		
	ImGui::Text("mousePositon x:%f y:%f", input_->GetMousePosition().x, input_->GetMousePosition().y);

	/*
	if (ImGui::Button("BGMstop")) {
		audio_->SoundStopWave(bgm1_);
	}
	*/
	ImGui::End();
		

#endif // _DEBUG
}

void GameScene::Draw() {

	//Spriteの背景描画前処理
	//spritePlatform_->PreBackGroundDraw();

	//sprite_->Draw();

	//Modelの描画前処理
	modelPlatform_->PreDraw();
	//modelPlatform_->SkinPreDraw();

    // 全ストーンを描画
    for (auto& stone : stones_) {
        stone->Draw(mainCamera_);
    }

	//Spriteの描画前処理
	//spritePlatform_->PreDraw();

	//ParticleManager::GetInstance()->Draw();

    

    // (残りの描画処理は省略)
}

void GameScene::Finalize() {
    // 必要な後処理を行う
}