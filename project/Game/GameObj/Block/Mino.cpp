#include "Mino.h"

#include "Engine/Particle/ParticleManager.h"
#include "Engine/Editor/JsonSerializer.h"
#include "Engine/Math/Random/Random.h"

#include "Game/Collider/CollisionManager.h"

Mino::Mino() {}

Mino::~Mino() {}

void Mino::Initialize() {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");
}

void Mino::Update() {

	for (auto& block : blocks_) {
		block->Update();
	}

	collider_->SetPos(model_->GetWorldPos());
	collider_->InfoUpdate();
	if (cMana_) {
		cMana_->AddCollider(collider_.get());
	}
}

void Mino::Draw([[maybe_unused]] Material* mate, [[maybe_unused]] bool is) {
	OriginGameObject::Draw(mate, is);
	for (auto& block : blocks_) {
		block->Draw();
	}

#ifdef _DEBUG
	collider_->DrawCollider();
#endif // _DEBUG
}

void Mino::DebugGUI() {
#ifdef _DEBUG

#endif // _DEBUG
}

void Mino::DrawLine() {
#ifdef _DEBUG
	for (auto& block : blocks_) {
		block->DrawLine();
	}
	collider_->DrawCollider();
#endif // _DEBUG
}

void Mino::InitBlock(BlockType type) {
	blockType_ = type;
	blockMode_ = BlockMode::Fall;

	collider_ = std::make_unique<AABBCollider>();
	collider_->SetTag("block");
	collider_->SetWidth(2.0f);
	collider_->SetDepth(2.0f);
	collider_->SetHeight(2.0f);
	collider_->SetColliderType(ColliderType::Type_Block);
	collider_->SetTargetType(ColliderType::Type_Player);
	collider_->SetCollisionEnterCallback([this](const ColliderInfo& other) {OnCollisionEnter(other); });
	collider_->SetCollisionStayCallback([this](const ColliderInfo& other) {OnCollisionStay(other); });
	collider_->SetCollisionExitCallback([this](const ColliderInfo& other) {OnCollisionExit(other); });

	Vector4 color = { 1.0f,0.65f,0.0f,1.0f };
	switch (blockType_) {

	case BlockType::L:
		color = { 1.0f,0.65f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
      block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&model_->transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,4.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		model_->SetColor(color);
		break;
	case BlockType::T:
		color = { 0.55f,0.0f,0.55f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
      block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&model_->transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { -2.0f,0.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		model_->SetColor(color);
		break;
	case BlockType::S:
		color = { 0.0f,0.5f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
      block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&model_->transform);
			if (i == 0) block->GetModel()->transform.translate = { -2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 2.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		model_->SetColor(color);
		break;
	case BlockType::Z:
		color = { 1.0f,0.0f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
      block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&model_->transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { -2.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		model_->SetColor(color);
		break;
	case BlockType::O:
		color = { 1.0f,1.0f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
      block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&model_->transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 2.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		model_->SetColor(color);
		break;
	case BlockType::J:
		color = { 0.0f,0.0f,1.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
      block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&model_->transform);
			if (i == 0) block->GetModel()->transform.translate = { -2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,4.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		model_->SetColor(color);
		break;
	default:
		break;

	}
}

void Mino::OnCollisionEnter([[maybe_unused]] const ColliderInfo& other) {}

void Mino::OnCollisionStay([[maybe_unused]] const ColliderInfo& other) {}

void Mino::OnCollisionExit([[maybe_unused]] const ColliderInfo& other) {}

void Mino::SetCollisionMana(CollisionManager* cMana) {
	cMana_ = cMana;
}
