#include "Climber.h"

Climber::Climber() {
	
}

void Climber::Initialize() {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");

	// 初期位置を設定
	model_->transform.translate = kStartPos_;
}

void Climber::Update() {

}

void Climber::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	OriginGameObject::Draw(mate, is);

#ifdef _DEBUG

#endif // _DEBUG
}

void Climber::DebugGUI() {
#ifdef _DEBUG

#endif // _DEBUG
}

