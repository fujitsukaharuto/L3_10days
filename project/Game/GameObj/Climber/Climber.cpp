#include "Climber.h"

Climber::Climber() {

}

void Climber::Initialize() {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");

	// 初期位置を設定
	model_->transform.translate = kStartPos_;

	// 向きを設定
	dir_ = ClimberDir::RIGHT;
	// 待機状態
	state_ = ClimberState::IDLE;

	// コライダー生成
	frontCollider_ = std::make_unique<ClimberFrontCollider>(this);
	frontCollider_->Initialize();

	upCollider_ = std::make_unique<ClimberTopCollider>(this);
	upCollider_->Initialize();

}

void Climber::Update() {
	// 移動処理
	if (state_ == ClimberState::MOVE) {
		if (dir_ == ClimberDir::LEFT) {
			model_->transform.translate.x -= speed_ * FPSKeeper::DeltaTime();
			model_->transform.rotate.y = std::numbers::pi_v<float>;
		} else {
			model_->transform.translate.x += speed_ * FPSKeeper::DeltaTime();
			model_->transform.rotate.y = 0.0f;
		}
	}

	// 壁による反転処理
	if (model_->transform.translate.x < leftWall_ || model_->transform.translate.x > rightWall_) {
		Turn();
	}

	// コライダー更新
	frontCollider_->Update();
	upCollider_->Update();

}

void Climber::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	OriginGameObject::Draw(mate, is);

#ifdef _DEBUG
	frontCollider_->Draw();
	upCollider_->Draw();
#endif // _DEBUG
}

void Climber::DebugGUI() {
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Climber")) {
		if (ImGui::Button("MOVE")) {
			state_ = ClimberState::MOVE;
		}
	}
#endif // _DEBUG
}


void Climber::Turn() {
	if (dir_ == ClimberDir::LEFT) {
		dir_ = ClimberDir::RIGHT;
	} else {
		dir_ = ClimberDir::LEFT;
	}
}

void Climber::Up() {

}
