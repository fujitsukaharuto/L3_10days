#include "GameOverScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "CameraManager.h"
#include "FPSKeeper.h"
#include "Math/Random/Random.h"
#include "Engine/Light/LightManager.h"

#include "Particle/ParticleManager.h"
#include "Scene/SceneManager.h"



GameOverScene::GameOverScene() {}

GameOverScene::~GameOverScene() {
}

void GameOverScene::Initialize() {

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
	black_->Load("sceneMove.png");
	black_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	black_->SetPos({ 0.0f,0.0f,0.0f });
	black_->SetAnchor({ 0.0f,0.0f });
#pragma endregion

	sphere = std::make_unique<Object3d>();
	sphere->CreateSphere();
	sphere->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	goTitle_ = std::make_unique<Sprite>();
	goTitle_->Load("titleReturn.png");
	goTitle_->SetPos({ 1050.0f,380.0f,0.0f });

	retry_ = std::make_unique<Sprite>();
	retry_->Load("tryAgain.png");
	retry_->SetPos({ 820.0f,270.0f,0.0f });

	chain_ = std::make_unique<Sprite>();
	chain_->Load("chain.png");
	chain_->SetPos({ 820.0f,-30.0f,0.0f });

	chain2_ = std::make_unique<Sprite>();
	chain2_->Load("chain.png");
	chain2_->SetPos({ 1050.0f,80.0f,0.0f });

	report_ = std::make_unique<Sprite>();
	report_->Load("gameoverReport.png");
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
	cursorTex_->SetSize({ 40.0f,40.0f });
	Vector2 mouse = Input::GetInstance()->GetMousePosition();
	cursorTex_->SetPos({ mouse.x,mouse.y,0.0f });

	man1 = std::make_unique<AnimationModel>();
	man1->Create("manWalk.gltf");
	man1->LoadAnimationFile("manLose.gltf");
	man1->transform.translate = { 4.0f,1.0f,5.0f };
	man1->transform.rotate.y = 3.14f;

	man2 = std::make_unique<AnimationModel>();
	man2->Create("manWalk2.gltf");
	man2->LoadAnimationFile("manLose2.gltf");
	man2->transform.translate = { -2.0f,1.0f,5.0f };
	man2->transform.rotate.y = 3.14f;

	half = std::make_unique<AnimationModel>();
	half->Create("halfWalk.gltf");
	half->LoadAnimationFile("halfLose.gltf");
	half->transform.translate = { 1.0f,1.0f,1.0f };
	half->transform.rotate.y = 3.14f;

	woman1 = std::make_unique<AnimationModel>();
	woman1->Create("womanWalk.gltf");
	woman1->LoadAnimationFile("womanLose.gltf");
	woman1->transform.translate = { 7.0f,1.5f,9.0f };
	woman1->transform.rotate.y = 3.14f;

	woman2 = std::make_unique<AnimationModel>();
	woman2->Create("womanWalk2.gltf");
	woman2->LoadAnimationFile("womanLose2.gltf");
	woman2->transform.translate = { -5.0f,1.5f,9.0f };
	woman2->transform.rotate.y = 3.14f;

	man1_1 = std::make_unique<AnimationModel>();
	man1_1->Create("manWalk.gltf");
	man1_1->LoadAnimationFile("manLose.gltf");
	man1_1->transform.translate = { -4.5f,-2.8f,-1.4f };
	man1_1->transform.rotate.z = -0.34f;
	man1_1->transform.rotate.y = 3.14f;

	man2_1 = std::make_unique<AnimationModel>();
	man2_1->Create("manWalk2.gltf");
	man2_1->LoadAnimationFile("manLose.gltf");
	man2_1->transform.translate = { -9.2f,13.0f,16.2f };
	man2_1->transform.rotate.z = 3.34f;
	man2_1->transform.rotate.y = 3.14f;

	half_1 = std::make_unique<AnimationModel>();
	half_1->Create("halfWalk.gltf");
	half_1->LoadAnimationFile("halfLose.gltf");
	half_1->transform.translate = { 13.5f,11.0f,13.0f };
	half_1->transform.rotate.z = 2.8f;
	half_1->transform.rotate.y = 3.14f;

	woman1_1 = std::make_unique<AnimationModel>();
	woman1_1->Create("womanWalk.gltf");
	woman1_1->LoadAnimationFile("womanLose.gltf");
	woman1_1->transform.translate = { -8.0f, 1.5f,1.0f };
	woman1_1->transform.rotate.z = -0.20f;
	woman1_1->transform.rotate.y = 3.14f;

	woman2_1 = std::make_unique<AnimationModel>();
	woman2_1->Create("womanWalk2.gltf");
	woman2_1->LoadAnimationFile("womanLose2.gltf");
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

	SoundData& soundData1 = audioPlayer_->SoundLoadWave("loseBGM.wav");
	audioPlayer_->SoundLoop(soundData1);
}

void GameOverScene::Update() {

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
		float posY = std::lerp(-60.0f, 380.0f, t);
		goTitle_->SetPos({ 1050.0f,posY,0.0f });
		posY = std::lerp(-360.0f, 80.0f, t);
		chain2_->SetPos({ 1050.0f,posY,0.0f });
		posY = std::lerp(-60.0f, 270.0f, t);
		retry_->SetPos({ 820.0f,posY,0.0f });
		posY = std::lerp(-360.0f, -30.0f, t);
		chain_->SetPos({ 820.0f,posY,0.0f });

		float t2 = 1.0f - std::pow(1.0f - x, 5.0f);
		if (x == 1.0f) { t2 = 1.0f; }
		float posX = std::lerp(-180.0f, 180.0f, t2);
		report_->SetPos({ posX,470.0f,0.0f });
		float angle = std::lerp(-0.38f, 0.38f, t2);
		report_->SetAngle(angle);

		if (goTitleTime_ == 90.0f) {
			goTitle_->SetPos({ 1050.0f,380.0f,0.0f });
			chain2_->SetPos({ 1050.0f,80.0f,0.0f });

			retry_->SetPos({ 820.0f,270.0f,0.0f });
			chain_->SetPos({ 820.0f,-30.0f,0.0f });

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
			if (blackTime == 0.0f && goTitleReturenTime_ == 0.0f) {
				goTitleReturenTime_ += 0.01f;
				SoundData& soundData1 = audioPlayer_->SoundLoadWave("push.wav");
				audioPlayer_->SoundPlayWave(soundData1);
			}
		}
		goTitle_->SetColor({ 0.4f,0.4f,0.4f,1.0f });
	} else {
		goTitle_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	pos = retry_->GetPos();   // 中心座標
	size = { 330.0f,80.0f }; // 幅・高さ
	halfW = size.x * 0.5f;
	halfH = size.y * 0.5f;
	if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
		mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
		if (Input::GetInstance()->IsTriggerMouse(0)) {
			if (blackTime == 0.0f && goTitleReturenTime_ == 0.0f) {
				goTitleReturenTime_ += 0.01f;
				isRetry_ = true;
				SoundData& soundData1 = audioPlayer_->SoundLoadWave("push.wav");
				audioPlayer_->SoundPlayWave(soundData1);
			}
		}
		retry_->SetColor({ 0.4f,0.4f,0.4f,1.0f });
	} else {
		retry_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
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
		if (isRetry_) {
			float posY = std::lerp(270.0f, -60.0f, t);
			retry_->SetPos({ 820.0f,posY,0.0f });
			posY = std::lerp(-30.0f, -360.0f, t);
			chain_->SetPos({ 820.0f,posY,0.0f });
		} else {
			float posY = std::lerp(380.0f, -60.0f, t);
			goTitle_->SetPos({ 1050.0f,posY,0.0f });
			posY = std::lerp(80.0f, -360.0f, t);
			chain2_->SetPos({ 1050.0f,posY,0.0f });
		}

		if (goTitleReturenTime_ == 90.0f) {
			if (isRetry_) {
				isGoRetry_ = true;
				retry_->SetPos({ 820.0f,-60.0f,0.0f });
				chain_->SetPos({ 820.0f,-360.0f,0.0f });
			} else {
				isGoTitle_ = true;
				goTitle_->SetPos({ 1050.0f,-60.0f,0.0f });
				chain2_->SetPos({ 1050.0f,-360.0f,0.0f });
			}
		}
	}


	cursorTex_->SetPos({ mouse.x,mouse.y,0.0f });

	ParticleManager::GetInstance()->Update();
}

void GameOverScene::Draw() {

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
	chain2_->Draw();
	goTitle_->Draw();
	retry_->Draw();

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

void GameOverScene::DebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();

	if (ImGui::CollapsingHeader("Sphere")) {
		ImGui::DragFloat3("scale", &sphere->transform.scale.x, 0.01f);
		ImGui::DragFloat3("rotate", &sphere->transform.rotate.x, 0.01f);
	}

	ImGui::Unindent();
#endif // _DEBUG
}

void GameOverScene::ParticleDebugGUI() {
#ifdef _DEBUG
	ImGui::Indent();

	ImGui::Unindent();
#endif // _DEBUG
}

void GameOverScene::BlackFade() {
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
			if (isRetry_) {
				ChangeScene("TITLE", 40.0f);
			} else {
				ChangeScene("GAME", 40.0f);
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

	if (isGoTitle_) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
		}
	}
	if (isGoRetry_) {
		if (blackTime == 0.0f) {
			isChangeFase = true;
		}
	}
}

void GameOverScene::ApplyGlobalVariables() {


}
