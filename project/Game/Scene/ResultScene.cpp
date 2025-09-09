#include "ResultScene.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "GlobalVariables.h"
#include "CameraManager.h"
#include "FPSKeeper.h"
#include "Math/Random/Random.h"

#include "Particle/ParticleManager.h"
#include "Scene/SceneManager.h"



ResultScene::ResultScene() {}

ResultScene::~ResultScene() {
}

void ResultScene::Initialize() {

	obj3dCommon.reset(new Object3dCommon());
	obj3dCommon->Initialize();

	CameraManager::GetInstance()->GetCamera()->transform.rotate = { -1.02f,0.0f,0.0f };
	CameraManager::GetInstance()->GetCamera()->transform.translate = { 0.0f, 3.5f, -20.0f };

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

}

void ResultScene::Update() {

#ifdef _DEBUG


#endif // _DEBUG

	BlackFade();

	sphere->transform.rotate.y += 0.02f;

	Vector2 mouse = Input::GetInstance()->GetMousePosition();
	cursorTex_->SetPos({ mouse.x,mouse.y,0.0f });

	ParticleManager::GetInstance()->Update();
}

void ResultScene::Draw() {

#pragma region 背景描画

	back_->Draw();
	frame_->Draw();
	report_->Draw();
	chain_->Draw();
	goTitle_->Draw();
	dxcommon_->ClearDepthBuffer();
#pragma endregion


#pragma region 3Dオブジェクト
	obj3dCommon->PreDraw();
	sphere->Draw();


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
			blackTime -= FPSKeeper::DeltaTime();
			if (blackTime <= 0.0f) {
				blackTime = 0.0f;
			}
		}
	}
	black_->SetColor({ 0.0f,0.0f,0.0f,Lerp(0.0f,1.0f,(1.0f / blackLimmite * blackTime)) });
	Vector2 mouse = Input::GetInstance()->GetMousePosition();
	Vector3 pos = goTitle_->GetPos();   // 中心座標
	Vector2 size = { 330.0f,80.0f }; // 幅・高さ
	float halfW = size.x * 0.5f;
	float halfH = size.y * 0.5f;
	if (mouse.x >= pos.x - halfW && mouse.x <= pos.x + halfW &&
		mouse.y >= pos.y - halfH && mouse.y <= pos.y + halfH) {
		if (Input::GetInstance()->IsTriggerMouse(0)) {
			if (blackTime == 0.0f) {
				isChangeFase = true;
			}
		}
		goTitle_->SetColor({ 0.4f,0.4f,0.4f,1.0f });
	} else {
		goTitle_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
}

void ResultScene::ApplyGlobalVariables() {


}
