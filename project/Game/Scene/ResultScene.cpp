#include "ResultScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "CameraManager.h"
#include "FPSKeeper.h"
#include "Math/Random/Random.h"
#include "Engine/Light/LightManager.h"

#include "Particle/ParticleManager.h"
#include "Scene/SceneManager.h"



ResultScene::ResultScene() {}

ResultScene::~ResultScene() {
}

void ResultScene::Initialize() {

	obj3dCommon.reset(new Object3dCommon());
	obj3dCommon->Initialize();

	CameraManager::GetInstance()->GetCamera()->transform.rotate = { 0.0f,0.0f,0.0f };
	CameraManager::GetInstance()->GetCamera()->transform.translate = { 0.0f, 3.5f, -20.0f };
	CameraManager::GetInstance()->GetCamera()->SetIsHeiko(false);
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->intensity = 1.5f;
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->direction = { 0.0f,-0.8f,0.6f };
	ModelManager::GetInstance()->ShareLight()->GetDirectionLight()->directionLightData_->color = { 1.0f,0.938f,0.671f,1.0f };

#pragma region シーン遷移用
	black_ = std::make_unique<Sprite>();
	black_->Load("white2x2.png");
	black_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	black_->SetSize({ 1280.0f,720.0f });
	black_->SetAnchor({ 0.0f,0.0f });
#pragma endregion

	sphere = std::make_unique<Object3d>();
	sphere->CreateSphere();
	sphere->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	goTitle_ = std::make_unique<Sprite>();
	goTitle_->Load("titleReturn.png");
	goTitle_->SetPos({ 980.0f,280.0f,0.0f });

	chain_ = std::make_unique<Sprite>();
	chain_->Load("chain.png");
	chain_->SetPos({ 980.0f,-20.0f,0.0f });

	report_ = std::make_unique<Sprite>();
	report_->Load("clearReport.png");
	report_->SetAngle(0.38f);
	report_->SetPos({ 180.0f,470.0f,0.0f });

	frame_ = std::make_unique<Sprite>();
	frame_->Load("resultFrame.png");
	frame_->SetPos({ 640.0f,360.0f,0.0f });

	back_ = std::make_unique<Sprite>();
	back_->Load("resultBackground.png");
	back_->SetPos({ 640.0f,360.0f,0.0f });

	cursorTex_ = std::make_unique<Sprite>();
	cursorTex_->Load("normalCursor.png");
	cursorTex_->SetAnchor({ 0.25f,0.25f });
	Vector2 mouse = Input::GetInstance()->GetMousePosition();
	cursorTex_->SetPos({ mouse.x,mouse.y,0.0f });


	man1 = std::make_unique<AnimationModel>();
	man1->Create("manWalk.gltf");
	man1->LoadAnimationFile("manWin.gltf");
	man1->transform.translate = { 4.0f,1.0f,5.0f };
	man1->transform.rotate.y = 3.14f;

	man2 = std::make_unique<AnimationModel>();
	man2->Create("manWalk2.gltf");
	man2->LoadAnimationFile("manWin.gltf");
	man2->transform.translate = { -2.0f,1.0f,5.0f };
	man2->transform.rotate.y = 3.14f;

	half = std::make_unique<AnimationModel>();
	half->Create("halfWalk.gltf");
	half->LoadAnimationFile("halfWin.gltf");
	half->transform.translate = { 1.0f,1.0f,1.0f };
	half->transform.rotate.y = 3.14f;

	woman1 = std::make_unique<AnimationModel>();
	woman1->Create("womanWalk.gltf");
	woman1->LoadAnimationFile("womanWin.gltf");
	woman1->transform.translate = { 7.0f,1.5f,9.0f };
	woman1->transform.rotate.y = 3.14f;

	woman2 = std::make_unique<AnimationModel>();
	woman2->Create("womanWalk2.gltf");
	woman2->LoadAnimationFile("womanWin2.gltf");
	woman2->transform.translate = { -5.0f,1.5f,9.0f };
	woman2->transform.rotate.y = 3.14f;

	man1_1 = std::make_unique<AnimationModel>();
	man1_1->Create("manWalk.gltf");
	man1_1->LoadAnimationFile("manWin.gltf");
	man1_1->transform.translate = { -4.5f,-2.8f,-1.4f };
	man1_1->transform.rotate.z = -0.34f;
	man1_1->transform.rotate.y = 3.14f;

	man2_1 = std::make_unique<AnimationModel>();
	man2_1->Create("manWalk2.gltf");
	man2_1->LoadAnimationFile("manWin.gltf");
	man2_1->transform.translate = { -9.2f,13.0f,16.2f };
	man2_1->transform.rotate.z = 3.34f;
	man2_1->transform.rotate.y = 3.14f;

	half_1 = std::make_unique<AnimationModel>();
	half_1->Create("halfWalk.gltf");
	half_1->LoadAnimationFile("halfWin.gltf");
	half_1->transform.translate = { 13.5f,11.0f,13.0f };
	half_1->transform.rotate.z = 2.8f;
	half_1->transform.rotate.y = 3.14f;

	woman1_1 = std::make_unique<AnimationModel>();
	woman1_1->Create("womanWalk.gltf");
	woman1_1->LoadAnimationFile("womanWin.gltf");
	woman1_1->transform.translate = { -8.5f,-0.5f,1.0f };
	woman1_1->transform.rotate.z = -0.20f;
	woman1_1->transform.rotate.y = 3.14f;

	woman2_1 = std::make_unique<AnimationModel>();
	woman2_1->Create("womanWalk2.gltf");
	woman2_1->LoadAnimationFile("womanWin2.gltf");
	woman2_1->transform.translate = { 8.0f, -3.0f, 1.5f };
	woman2_1->transform.rotate.z = 0.18f;
	woman2_1->transform.rotate.y = 3.14f;

	man1->RandomAddAnimationTime();
	man2->RandomAddAnimationTime();
	half->RandomAddAnimationTime();
	woman1->RandomAddAnimationTime();
	woman2->RandomAddAnimationTime();
	man1_1->RandomAddAnimationTime();
	man2_1->RandomAddAnimationTime();
	half_1->RandomAddAnimationTime();
	woman1_1->RandomAddAnimationTime();
	woman2_1->RandomAddAnimationTime();

	SoundData& soundData1 = audioPlayer_->SoundLoadWave("winBGM.wav");
	audioPlayer_->SoundLoop(soundData1);
}

void ResultScene::Update() {

#ifdef _DEBUG


#endif // _DEBUG

	BlackFade();

	//sphere->transform.rotate.y += 0.02f;

	man1->AnimationUpdate();
	man2->AnimationUpdate();
	half->AnimationUpdate();
	woman1->AnimationUpdate();
	woman2->AnimationUpdate();
	man1_1->AnimationUpdate();
	man2_1->AnimationUpdate();
	half_1->AnimationUpdate();
	woman1_1->AnimationUpdate();
	woman2_1->AnimationUpdate();

	if (goTitleTime_ < 90.0f) {
		if (FPSKeeper::DeltaTime() < 3.0f) {
			goTitleTime_ += FPSKeeper::DeltaTime();
		}
		goTitleTime_ = std::clamp(goTitleTime_, 0.0f, 90.0f);

		const float c4 = (2.0f * 3.14159265359f) / 3.0f; // 2π/3
		float x = (goTitleTime_ / 90.0f);
		float t = 0.0f;
		if ((x / 40.0f) == 0.0f) {
			t = 0.0f;
		} else if (x == 1.0f) {
			t = 1.0f;
		} else {
			t = powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
		}
		float posY = std::lerp(-60.0f, 280.0f, t);
		goTitle_->SetPos({ 980.0f,posY,0.0f });
		posY = std::lerp(-360.0f, -20.0f, t);
		chain_->SetPos({ 980.0f,posY,0.0f });

		float t2 = 1.0f - std::pow(1.0f - x, 5.0f);
		if (x == 1.0f) { t2 = 1.0f; }
		float posX = std::lerp(-180.0f, 180.0f, t2);
		report_->SetPos({ posX,470.0f,0.0f });
		float angle = std::lerp(-0.38f, 0.38f, t2);
		report_->SetAngle(angle);

		if (goTitleTime_ == 90.0f) {
			goTitle_->SetPos({ 980.0f,280.0f,0.0f });
			chain_->SetPos({ 980.0f,-20.0f,0.0f });

			report_->SetAngle(0.38f);
			report_->SetPos({ 180.0f,470.0f,0.0f });
		}
	}


	Vector2 mouse = Input::GetInstance()->GetMousePosition();
	Vector3 pos = goTitle_->GetPos();   // 中心座標
	Vector2 size = { 330.0f,80.0f }; // 幅・高さ
	float halfW = size.x * 0.5f;
	float halfH = size.y * 0.5f;
	if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
		mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
		if (Input::GetInstance()->IsTriggerMouse(0)) {
			if (blackTime == 0.0f) {
				goTitleReturenTime_ += 0.01f;
				SoundData& soundData1 = audioPlayer_->SoundLoadWave("push.wav");
				audioPlayer_->SoundPlayWave(soundData1);
			}
		}
		goTitle_->SetColor({ 0.4f,0.4f,0.4f,1.0f });
	} else {
		goTitle_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}


	if (goTitleReturenTime_ < 90.0f && goTitleReturenTime_ > 0.0f) {
		if (FPSKeeper::DeltaTime() < 3.0f) {
			goTitleReturenTime_ += FPSKeeper::DeltaTime();
		}
		goTitleReturenTime_ = std::clamp(goTitleReturenTime_, 0.0f, 90.0f);

		const float c5 = (2.0f * 3.14159265f) / 4.5f;

		float x = (goTitleReturenTime_ / 90.0f);
		float t = 0.0f;
		if (x == 0.0f) {
			t = 0.0f;
		} else if (x == 1.0f) {
			t = 1.0f;
		} else if (x < 0.5f) {
			t = -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f;
		} else {
			t = (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
		}
		float posY = std::lerp(280.0f, -60.0f, t);
		goTitle_->SetPos({ 980.0f,posY,0.0f });
		posY = std::lerp(-20.0f, -360.0f, t);
		chain_->SetPos({ 980.0f,posY,0.0f });

		if (goTitleReturenTime_ == 90.0f) {
			isGoTitle_ = true;
			goTitle_->SetPos({ 980.0f,-60.0f,0.0f });
			chain_->SetPos({ 980.0f,-360.0f,0.0f });
		}
	}



	cursorTex_->SetPos({ mouse.x,mouse.y,0.0f });

	ParticleManager::GetInstance()->Update();
}

void ResultScene::Draw() {

#pragma region 背景描画

	man1->CSDispatch();
	man2->CSDispatch();
	half->CSDispatch();
	woman1->CSDispatch();
	woman2->CSDispatch();
	man1_1->CSDispatch();
	man2_1->CSDispatch();
	half_1->CSDispatch();
	woman1_1->CSDispatch();
	woman2_1->CSDispatch();

	back_->Draw();
	dxcommon_->ClearDepthBuffer();
#pragma endregion


#pragma region 3Dオブジェクト
	obj3dCommon->PreDraw();
	//sphere->Draw();

	man1->Draw();
	man2->Draw();
	half->Draw();
	woman1->Draw();
	woman2->Draw();
	man1_1->Draw();
	man2_1->Draw();
	half_1->Draw();
	woman1_1->Draw();
	woman2_1->Draw();

	frame_->Draw();
	report_->Draw();
	chain_->Draw();
	goTitle_->Draw();

	ParticleManager::GetInstance()->Draw();

#pragma endregion


#pragma region 前景スプライト

	dxcommon_->PreSpriteDraw();
	if (blackTime != 0.0f) {
		black_->Draw();
	}
	cursorTex_->Draw();
#pragma endregion
	ModelManager::GetInstance()->PickingDataCopy();
}

void ResultScene::DebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();

	if (ImGui::CollapsingHeader("Sphere")) {
		ImGui::DragFloat3("scale", &sphere->transform.scale.x, 0.01f);
		ImGui::DragFloat3("rotate", &sphere->transform.rotate.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("man1")) {
		man1_1->DebugGUI();
	}
	if (ImGui::CollapsingHeader("man2")) {
		man2_1->DebugGUI();
	}
	if (ImGui::CollapsingHeader("half")) {
		half->DebugGUI();
	}
	if (ImGui::CollapsingHeader("woman1")) {
		woman1->DebugGUI();
	}
	if (ImGui::CollapsingHeader("woman2")) {
		woman2->DebugGUI();
	}

	ImGui::Unindent();
#endif // _DEBUG
}

void ResultScene::ParticleDebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();

	ImGui::Unindent();
#endif // _DEBUG
}

void ResultScene::BlackFade() {
	if (isChangeFase) {
		if (blackTime < blackLimmite) {
			blackTime += FPSKeeper::DeltaTime();
			if (blackTime >= blackLimmite) {
				blackTime = blackLimmite;
			}
		} else {
			ChangeScene("GAME", 40.0f);
		}
	} else {
		if (blackTime > 0.0f) {
			if (FPSKeeper::DeltaTime() < 3.0f) {
				blackTime -= FPSKeeper::DeltaTime();
			}
			if (blackTime <= 0.0f) {
				blackTime = 0.0f;
			}
		}
	}
	black_->SetColor({ 0.0f,0.0f,0.0f,Lerp(0.0f,1.0f,(1.0f / blackLimmite * blackTime)) });
	if (isGoTitle_) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
		}
	}
}

void ResultScene::ApplyGlobalVariables() {


}
