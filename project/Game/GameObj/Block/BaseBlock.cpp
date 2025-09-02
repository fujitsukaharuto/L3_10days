#include "BaseBlock.h"

#include "Engine/Particle/ParticleManager.h"
#include "Engine/Editor/JsonSerializer.h"
#include "Engine/Math/Random/Random.h"

#include "Game/Collider/CollisionManager.h"

BaseBlock::BaseBlock() {}

BaseBlock::~BaseBlock() {}

void BaseBlock::Initialize() {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");

	collider_ = std::make_unique<AABBCollider>();
	collider_->SetTag("block");
	collider_->SetWidth(2.0f);
	collider_->SetDepth(2.0f);
	collider_->SetHeight(2.0f);
	collider_->SetCollisionEnterCallback([this](const ColliderInfo& other) {OnCollisionEnter(other); });
	collider_->SetCollisionStayCallback([this](const ColliderInfo& other) {OnCollisionStay(other); });
	collider_->SetCollisionExitCallback([this](const ColliderInfo& other) {OnCollisionExit(other); });
}

void BaseBlock::Update() {
	collider_->SetPos(model_->GetWorldPos());
	collider_->InfoUpdate();
	if (cMana_) {
		cMana_->AddCollider(collider_.get());
	}
}

void BaseBlock::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	OriginGameObject::Draw(mate, is);

#ifdef _DEBUG
	collider_->DrawCollider();
#endif // _DEBUG
}

void BaseBlock::DebugGUI() {
#ifdef _DEBUG

#endif // _DEBUG
}


void BaseBlock::DrawLine() {
#ifdef _DEBUG
	collider_->DrawCollider();
#endif // _DEBUG
}

void BaseBlock::OnCollisionEnter([[maybe_unused]] const ColliderInfo& other) {
}

void BaseBlock::OnCollisionStay([[maybe_unused]] const ColliderInfo& other) {}

void BaseBlock::OnCollisionExit([[maybe_unused]] const ColliderInfo& other) {}

void BaseBlock::SetCollisionManager(CollisionManager* cMana) {
	cMana_ = cMana;
}
