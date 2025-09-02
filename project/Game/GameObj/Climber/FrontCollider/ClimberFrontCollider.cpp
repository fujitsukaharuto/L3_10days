#include "ClimberFrontCollider.h"

#include "GameObj/Climber/Climber.h"

ClimberFrontCollider::ClimberFrontCollider(Climber* climber) {
	climber_ = climber;
}

void ClimberFrontCollider::Initialize() {
	OriginGameObject::Initialize();

	// 親子関係セット
	model_->transform.parent = &climber_->GetTrans();

	// 座標をセット
	model_->transform.translate.x = 1.0f;

	// コライダー用のオブジェクトを実装
	collider_ = std::make_unique<AABBCollider>();
	collider_->SetTag("climber");
	collider_->SetWidth(0.5f);
	collider_->SetDepth(0.5f);
	collider_->SetHeight(0.5f);

	collider_->SetCollisionEnterCallback([this](const ColliderInfo& other) {OnCollisionEnter(other); });
	collider_->SetCollisionStayCallback([this](const ColliderInfo& other) {OnCollisionStay(other); });
	collider_->SetCollisionExitCallback([this](const ColliderInfo& other) {OnCollisionExit(other); });
}

void ClimberFrontCollider::Update() {
	collider_->SetPos(model_->GetWorldPos());
	collider_->InfoUpdate();
}

void ClimberFrontCollider::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
#ifdef _DEBUG
	collider_->DrawCollider();
#endif // _DEBUG
}

void ClimberFrontCollider::DebugGUI() {

}

void ClimberFrontCollider::OnCollisionEnter([[maybe_unused]] const ColliderInfo& other) {
	// ブロックに衝突した場合
	if (other.tag == "block") {
		climber_->Turn();
	}
}

void ClimberFrontCollider::OnCollisionStay([[maybe_unused]] const ColliderInfo& other) {

}

void ClimberFrontCollider::OnCollisionExit([[maybe_unused]] const ColliderInfo& other) {

}