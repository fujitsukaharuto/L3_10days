#include "ClimberBottomCollider.h"

#include "GameObj/Climber/Climber.h"
#include "Game/Collider/CollisionManager.h"

ClimberBottomCollider::ClimberBottomCollider(Climber* climber, CollisionManager* cMana) {
	climber_ = climber;
	cMana_ = cMana;
}

void ClimberBottomCollider::Initialize() {
	OriginGameObject::Initialize();

	// 親子関係セット
	model_->transform.parent = &climber_->GetTrans();

	// 座標をセット
	model_->transform.translate.y = -1.15f;

	// コライダー用のオブジェクトを実装
	collider_ = std::make_unique<AABBCollider>();
	collider_->SetTag("climber");
	collider_->SetWidth(0.1f);
	collider_->SetDepth(0.1f);
	collider_->SetHeight(0.1f);

	collider_->SetCollisionEnterCallback([this](const ColliderInfo& other) {OnCollisionEnter(other); });
	collider_->SetCollisionStayCallback([this](const ColliderInfo& other) {OnCollisionStay(other); });
	collider_->SetCollisionExitCallback([this](const ColliderInfo& other) {OnCollisionExit(other); });
}

void ClimberBottomCollider::Update() {
	collider_->SetPos(model_->GetWorldPos());
	collider_->InfoUpdate();
	cMana_->AddCollider(collider_.get());
}

void ClimberBottomCollider::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
#ifdef _DEBUG
	collider_->DrawCollider();
#endif // _DEBUG
}

void ClimberBottomCollider::DebugGUI() {

}

void ClimberBottomCollider::OnCollisionEnter([[maybe_unused]] const ColliderInfo& other) {
	if (other.tag == "block") {
		climber_->ThereBottomBlock();
	}
}

void ClimberBottomCollider::OnCollisionStay([[maybe_unused]] const ColliderInfo& other) {
	if (other.tag == "block") {
		climber_->ThereBottomBlock();
	}
}

void ClimberBottomCollider::OnCollisionExit([[maybe_unused]] const ColliderInfo& other) {

}