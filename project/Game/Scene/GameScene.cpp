#include "GameScene.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include "FPSKeeper.h"
#include "Game/GameObj/FollowCamera.h"
#include "Engine/Model/ModelManager.h"
#include "Engine/Light/LightManager.h"

#include "Particle/ParticleManager.h"
#include "Scene/SceneManager.h"



GameScene::GameScene() {}

GameScene::~GameScene() {
}

void GameScene::Initialize() {

	//GlobalVariables* globalvariables = GlobalVariables::GetInstance();

#ifndef _DEBUG
	MyWin::GetInstance()->SetDrawCursor(false);
#endif // !_DEBUG

	CameraManager::GetInstance()->GetCamera()->transform.rotate = { 0.2f,0.0f,0.0f };
	CameraManager::GetInstance()->GetCamera()->transform.translate = { 20.0f, 30.0f, -70.0f };
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->intensity = 1.5f;
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->direction = { 0.0f,-0.8f,0.6f };
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->color = { 1.0f,0.938f,0.671f,1.0f };

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

	map_ = std::make_unique<MapField>();
	map_->Initialize();
	map_->TitleInit();
	map_->SetColliderManager(cMane_.get());

	/*title = std::make_unique<Object3d>();
	title->Create("title.obj");
	title->SetLightEnable(LightMode::kLightNone);
	title->transform.translate = { 39.0f,33.0f,0.0f };
	title->transform.scale = { 10.0f,10.0f,10.0f };
	title->transform.rotate.y = 3.14f;*/

	ni_ = std::make_unique<AnimationModel>();
	ni_->Create("title1.gltf");
	ni_->LoadAnimationFile("title1.gltf");
	ni_->transform.translate = { 22.1f,29.0f,-62.0f };
	ni_->transform.rotate.y = 3.14f;

	nn_ = std::make_unique<AnimationModel>();
	nn_->Create("title2.gltf");
	nn_->LoadAnimationFile("title2.gltf");
	nn_->transform.translate = { 22.1f,29.0f,-62.0f };
	nn_->transform.rotate.y = 3.14f;

	ge_ = std::make_unique<AnimationModel>();
	ge_->Create("title3.gltf");
	ge_->LoadAnimationFile("title3.gltf");
	ge_->transform.translate = { 22.1f,29.0f,-62.0f };
	ge_->transform.rotate.y = 3.14f;

	nn2_ = std::make_unique<AnimationModel>();
	nn2_->Create("title4.gltf");
	nn2_->LoadAnimationFile("title4.gltf");
	nn2_->transform.translate = { 22.1f,29.0f,-62.0f };
	nn2_->transform.rotate.y = 3.14f;

	factory_ = std::make_unique<AnimationModel>();
	factory_->Create("title5.gltf");
	factory_->LoadAnimationFile("title5.gltf");
	factory_->transform.translate = { 22.1f,29.0f,-62.0f };
	factory_->transform.rotate.y = 3.14f;

	ApplyGlobalVariables();

	cMane_ = std::make_unique<CollisionManager>();

	SoundData& soundData1 = audioPlayer_->SoundLoadWave("titleBGM.wav");
	audioPlayer_->SoundLoop(soundData1);
}

void GameScene::Update() {

	cMane_->Reset();

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	map_->TitleUpdate();

#ifdef _DEBUG

#endif // _DEBUG

	ni_->AnimationUpdate();
	nn_->AnimationUpdate();
	ge_->AnimationUpdate();
	nn2_->AnimationUpdate();
	factory_->AnimationUpdate();

	skybox_->Update();
	BlackFade();


	ParticleManager::GetInstance()->Update();
}

void GameScene::Draw() {
#pragma region 背景描画

	ni_->CSDispatch();
	nn_->CSDispatch();
	ge_->CSDispatch();
	nn2_->CSDispatch();
	factory_->CSDispatch();

	map_->BackDraw();
	dxcommon_->ClearDepthBuffer();
#pragma endregion


#pragma region 3Dオブジェクト
	obj3dCommon->PreDraw();
	dxcommon_->ClearDepthBuffer();
	map_->TitleDraw();
	dxcommon_->ClearDepthBuffer();

	obj3dCommon->PreDraw();

	//title->Draw();
	ni_->Draw();
	nn_->Draw();
	ge_->Draw();
	nn2_->Draw();
	factory_->Draw();


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
	map_->CursorDraw();

#pragma endregion
	ModelManager::GetInstance()->PickingDataCopy();
}

void GameScene::DebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();

	if (ImGui::CollapsingHeader("terrain")) {
		terrain->DebugGUI();
	}
	/*if (ImGui::CollapsingHeader("title")) {
		title->DebugGUI();
	}*/
	if (ImGui::CollapsingHeader("ni")) {
		ni_->DebugGUI();
	}
	if (ImGui::CollapsingHeader("nn")) {
		nn_->DebugGUI();
	}
	if (ImGui::CollapsingHeader("ge")) {
		ge_->DebugGUI();
	}
	if (ImGui::CollapsingHeader("nn2")) {
		nn2_->DebugGUI();
	}
	if (ImGui::CollapsingHeader("factory")) {
		factory_->DebugGUI();
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
				ChangeScene("TITLE", 40.0f);
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
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
		}
	}
#ifdef _DEBUG
	/*if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
		}
	}*/
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
