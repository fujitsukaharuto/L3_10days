#include "TitleScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "CameraManager.h"
#include "FPSKeeper.h"
#include "Math/Random/Random.h"
#include "Model/Line3dDrawer.h"
#include "Particle/ParticleManager.h"
#include "Scene/SceneManager.h"
#include "Engine/Editor/CommandManager.h"
#include "Engine/Light/LightManager.h"



TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize() {

	obj3dCommon.reset(new Object3dCommon());
	obj3dCommon->Initialize();

#ifndef _DEBUG
	MyWin::GetInstance()->SetDrawCursor(false);
#endif // !_DEBUG

	CameraManager::GetInstance()->GetCamera()->transform.rotate = { 0.01f,0.005f,0.0f };
	CameraManager::GetInstance()->GetCamera()->transform.translate = { 19.0f, 28.0f, -300.0f };
	CameraManager::GetInstance()->GetCamera()->SetIsHeiko(true);
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->intensity = 1.5f;
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->direction = { 0.0f,-0.8f,0.6f };
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->color = { 1.0f,0.938f,0.671f,1.0f };

	dxcommon_->GetOffscreenManager()->ResetPostEffect();
	//dxcommon_->GetOffscreenManager()->AddPostEffect(PostEffectList::Bloom);

	cMane_ = std::make_unique<CollisionManager>();

#pragma region シーン遷移用
	black_ = std::make_unique<Sprite>();
	black_->Load("sceneMove.png");
	black_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	black_->SetPos({ 0.0f,0.0f,0.0f });
	black_->SetAnchor({ 0.0f,0.0f });
#pragma endregion

	skybox_ = std::make_unique<SkyBox>();
	skybox_->SetCommonResources(dxcommon_, SRVManager::GetInstance(), CameraManager::GetInstance()->GetCamera());
	skybox_->Initialize();

	terrain_ = std::make_unique<AnimationModel>();
	terrain_->Create("ground.obj");
	terrain_->IsMirrorOBJ(true);
	terrain_->SetEnvironmentCoeff(0.3f);
	terrain_->SetTexture("grass.jpg");
	terrain_->transform.scale = { 1.0f,1.0f,1.0f };
	terrain_->SetUVScale({ 20.0f,20.0f }, { 0.0f,0.0f });

	map_ = std::make_unique<MapField>();
	map_->Initialize();

	terrainCollider_ = std::make_unique<AABBCollider>();
	terrainCollider_->SetTag("terrain");
	terrainCollider_->SetWidth(20.0f);
	terrainCollider_->SetDepth(20.0f);
	terrainCollider_->SetHeight(4.0f);

	friendlyManager_ = std::make_unique<FriendlyManager>();
	enemyManager_ = std::make_unique<EnemyManager>();

	battleSystem_ = std::make_unique<BattleSystem>(
		friendlyManager_.get(),
		enemyManager_.get()
	);

	map_->SetFriendlyManager(friendlyManager_.get());

	waveEditor_ = std::make_unique<WaveEditor>();
	enemyTableEditor_ = std::make_unique<EnemyTableEditor>();

	enemyManager_->SetWaveEditor(waveEditor_.get());
	enemyManager_->SetEnemyTableEditor(enemyTableEditor_.get());

	/*cube_ = std::make_unique<AnimationModel>();
	cube_->Create("T_boss.gltf");
	cube_->LoadAnimationFile("T_boss.gltf");
	cube_->LoadTransformFromJson("boss_transform.json");

	animParentObj_ = std::make_unique<Object3d>();
	animParentObj_->Create("boss.obj");
	animParentObj_->SetAnimParent(cube_->GetJointTrans("mixamorig:LeftHandIndex1"));
	animParentObj_->SetNoneScaleParent(true);
	animParentObj_->LoadTransformFromJson("AnimParent_transform.json");*/

#ifdef _DEBUG
	minoEditor_.Initialize();
#endif // _DEBUG

	ParticleManager::Load(emit, "lightning");

	SoundData& soundData1 = audioPlayer_->SoundLoadWave("gameBGM.wav");
	audioPlayer_->SoundLoop(soundData1);
}

void TitleScene::Update() {

	cMane_->Reset();

#ifdef _DEBUG



#endif // _DEBUG

	BlackFade();
	skybox_->Update();

	/*b1_->Update();
	b2_->Update();*/

	map_->Update();

	cMane_->CheckAllCollision();

	// 
	//　マネージャ更新
	// 

	if (!isEditorMode_) {
		friendlyManager_->Update();
		enemyManager_->Update();
		// 相手が死んでいた時にターゲットから外す処理
		friendlyManager_->CheckIsTargetDead();
		enemyManager_->CheckIsTargetDead();

		friendlyManager_->DeleteDeadObject();
		enemyManager_->DeleteDeadObject();

		battleSystem_->Update();

		if (battleSystem_->CheckBattleOver()) {
			isChangeFase = true;
		}
	}



	//climber_->Up();

	ParticleManager::GetInstance()->Update();
}

void TitleScene::Draw() {

	friendlyManager_->CSDispatch();
	enemyManager_->CSDispatch();

#pragma region 背景描画

	map_->BackDraw();
	dxcommon_->ClearDepthBuffer();
#pragma endregion


#pragma region 3Dオブジェクト
	//skybox_->Draw();
	obj3dCommon->PreDraw();
	terrain_->Draw();
	dxcommon_->ClearDepthBuffer();
	map_->FactoryDraw();
	dxcommon_->ClearDepthBuffer();

	obj3dCommon->PreDraw();

	map_->Draw();

	friendlyManager_->Draw();
	enemyManager_->Draw();

#ifdef _DEBUG
	CommandManager::GetInstance()->Draw();
#endif // _DEBUG

	ParticleManager::GetInstance()->Draw();

#ifdef _DEBUG
	emit.DrawSize();
#endif // _DEBUG

	//cube_->SkeletonDraw();
	Line3dDrawer::GetInstance()->Render();

#pragma endregion

	//test
#pragma region 前景スプライト

	dxcommon_->PreSpriteDraw();
	if (blackTime != 0.0f) {
		black_->Draw();
	}

	map_->CursorDraw();

#pragma endregion
	ModelManager::GetInstance()->PickingDataCopy();
}

void TitleScene::DebugGUI() {
#ifdef _DEBUG
	minoEditor_.DrawGUI();

	ImGui::Indent();
	map_->DebugGUI();
	friendlyManager_->DebugGUI();
	waveEditor_->Draw();
	enemyTableEditor_->Draw();
	EditorModeSwitchUI();
	ImGui::Unindent();
#endif // _DEBUG
}

void TitleScene::ParticleDebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();
	emit.DebugGUI();
	ImGui::Unindent();
#endif // _DEBUG
}

void TitleScene::BlackFade() {
	if (isChangeFase) {
		if (blackTime < blackLimmite) {
			blackTime += FPSKeeper::DeltaTime();
			float t = (blackTime / blackLimmite);
			float x = std::lerp(-1610.0f, 0.0f, t);
			black_->SetPos({ x,0.0f,0.0f });
			if (blackTime >= blackLimmite) {
				black_->SetPos({ 0.0f,0.0f,0.0f });
				blackTime = blackLimmite;
			}
		} else {
			if (!isParticleDebugScene_) {
				ChangeScene("RESULT", 40.0f);
			} else {
				ChangeScene("PARTICLEDEBUG", 40.0f);
			}
		}
	} else {
		if (blackTime > 0.0f) {
			if (FPSKeeper::DeltaTime() < 3.0f) {
				blackTime -= FPSKeeper::DeltaTime();
			}
			float t = 1.0f - (blackTime / blackLimmite);
			black_->SetPos({ 1290.0f * t,0.0f,0.0f });
			if (blackTime <= 0.0f) {
				black_->SetPos({ 1290.0f,0.0f,0.0f });
				blackTime = 0.0f;
			}
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
		}
	}
#ifdef _DEBUG
	if (Input::GetInstance()->PushKey(DIK_RETURN) && Input::GetInstance()->PushKey(DIK_P) && Input::GetInstance()->PushKey(DIK_D) && Input::GetInstance()->TriggerKey(DIK_S)) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
			isParticleDebugScene_ = true;
		}
	}
#endif // _DEBUG
}

void TitleScene::EditorModeSwitchUI() {
#ifdef _DEBUG

	ImGui::Begin("EditorModeSwitch");

	if (!isEditorMode_) {
		if (ImGui::Button("Enable")) {
			isEditorMode_ = !isEditorMode_;
		}
	} else {
		if (ImGui::Button("Disable")) {
			isEditorMode_ = !isEditorMode_;
		}
	}

	ImGui::End();
#endif
}

void TitleScene::ApplyGlobalVariables() {


}
