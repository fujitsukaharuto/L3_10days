#include "FightScene.h"
#include "CameraManager.h"

FightScene::FightScene() {}

FightScene::~FightScene() {}

void FightScene::Initialize() {
	CameraManager::GetInstance()->GetCamera()->transform.rotate = { 0.0f,0.0f,0.0f };
	CameraManager::GetInstance()->GetCamera()->transform.translate = { 20.0f, 22.0f, -75.0f };

	dxcommon_->GetOffscreenManager()->ResetPostEffect();

}

void FightScene::Update() {

}

void FightScene::Draw() {
	dxcommon_->ClearDepthBuffer();



}
