#include "GameScene.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include "FPSKeeper.h"
#include "Game/GameObj/FollowCamera.h"
#include "Engine/Model/ModelManager.h"

#include "Particle/ParticleManager.h"
#include "Scene/SceneManager.h"



GameScene::GameScene() {}

GameScene::~GameScene() {
}

void GameScene::Initialize() {

	GlobalVariables* globalvariables = GlobalVariables::GetInstance();

	obj3dCommon.reset(new Object3dCommon());
	obj3dCommon->Initialize();


#pragma region シーン遷移用
	black_ = std::make_unique<Sprite>();
	black_->Load("white2x2.png");
	black_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	black_->SetSize({ 1280.0f,720.0f });
	black_->SetAnchor({ 0.0f,0.0f });
#pragma endregion

	terrain = std::make_unique<AnimationModel>();
	terrain->Create("ground.obj");
	terrain->IsMirrorOBJ(true);
	terrain->SetEnvironmentCoeff(0.3f);
	terrain->SetTexture("grass.jpg");
	terrain->transform.scale = { 1.0f,1.0f,1.0f };
	terrain->SetUVScale({ 20.0f,20.0f }, { 0.0f,0.0f });

	skybox_ = std::make_unique<SkyBox>();
	skybox_->SetCommonResources(dxcommon_, SRVManager::GetInstance(), CameraManager::GetInstance()->GetCamera());
	skybox_->Initialize();

	ApplyGlobalVariables();

	cMane_ = std::make_unique<CollisionManager>();
}

void GameScene::Update() {

	cMane_->Reset();

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG



#ifdef _DEBUG
	
#endif // _DEBUG

	skybox_->Update();
	BlackFade();


	ParticleManager::GetInstance()->Update();
}

void GameScene::Draw() {
#pragma region 背景描画

	//boss_->CSDispatch();

	dxcommon_->ClearDepthBuffer();
#pragma endregion


#pragma region 3Dオブジェクト
	skybox_->Draw();


	obj3dCommon->PreDraw();


	terrain->Draw();

	ParticleManager::GetInstance()->Draw();

#ifdef _DEBUG

	Line3dDrawer::GetInstance()->Render();
#endif // _DEBUG
#pragma endregion


#pragma region 前景スプライト

	dxcommon_->PreSpriteDraw();
	//test->Draw();
	if (blackTime != 0.0f) {
		black_->Draw();
	}

#pragma endregion
	ModelManager::GetInstance()->PickingDataCopy();
}

void GameScene::DebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();

	if (ImGui::CollapsingHeader("terrain")) {
		terrain->DebugGUI();
	}

	ImGui::Unindent();
#endif // _DEBUG
}

void GameScene::ParticleDebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();

	ImGui::Unindent();
#endif // _DEBUG
}

void GameScene::BlackFade() {
	if (isChangeFase) {
		if (blackTime < blackLimmite) {
			blackTime += FPSKeeper::DeltaTime();
			if (blackTime >= blackLimmite) {
				blackTime = blackLimmite;
			}
		} else {
			if (isBackTitle_) {
				ChangeScene("TITLE", 40.0f);
			} else {
				ChangeScene("RESULT", 40.0f);
			}
		}
	} else {
		if (blackTime > 0.0f) {
			blackTime -= FPSKeeper::DeltaTime();
			if (blackTime <= 0.0f) {
				blackTime = 0.0f;
			}
		}
	}
	black_->SetColor({ 0.0f,0.0f,0.0f,Lerp(0.0f,1.0f,(1.0f / blackLimmite * blackTime)) });
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
		}
	}
#endif // _DEBUG
}

void GameScene::LoadSceneLevelData(const std::string& name) {
	BaseScene::LoadSceneLevelData(name);
	for (const auto& objJson : sceneData_["objects"]) {
		if (objJson.contains("objectType")) {
			if (objJson["objectType"] == "Normal") {
				
			} else if (objJson["objectType"] == "Player") {
				
			} else if (objJson["objectType"] == "Boss") {
				
			}
		}
	}
}

void GameScene::ApplyGlobalVariables() {
}
