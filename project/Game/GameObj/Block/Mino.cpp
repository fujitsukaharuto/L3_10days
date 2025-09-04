#include "Mino.h"

#include "Engine/Editor/JsonSerializer.h"
#include "Engine/Math/Random/Random.h"
#include "Engine/Particle/ParticleManager.h"

#include "Game/Collider/CollisionManager.h"

Mino::Mino() {}

Mino::~Mino() {}

void Mino::Initialize() {
	transform.scale = { 1.0f,1.0f,1.0f };
}

void Mino::Update() {
	for (auto& block : blocks_) {
		block->Update();
	}
}

void Mino::Draw() {
	for (auto& block : blocks_) {
		block->Draw();
	}
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
#endif // _DEBUG
}

void Mino::InitBlock(BlockType type) {
	blockType_ = type;
	blockMode_ = BlockMode::Fall;

	Vector4 color = { 1.0f,0.65f,0.0f,1.0f };

	switch (blockType_) {

	case BlockType::L:
		color = { 1.0f,0.65f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
			block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,4.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		break;
	case BlockType::T:
		color = { 0.55f,0.0f,0.55f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
			block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { -2.0f,0.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		break;
	case BlockType::S:
		color = { 0.0f,0.5f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
			block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&transform);
			if (i == 0) block->GetModel()->transform.translate = { -2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 2.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		break;
	case BlockType::Z:
		color = { 1.0f,0.0f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
			block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { -2.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		break;
	case BlockType::O:
		color = { 1.0f,1.0f,0.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
			block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&transform);
			if (i == 0) block->GetModel()->transform.translate = { 2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 2.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		break;
	case BlockType::J:
		color = { 0.0f,0.0f,1.0f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
			block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&transform);
			if (i == 0) block->GetModel()->transform.translate = { -2.0f,0.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,4.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		break;
	case BlockType::I:
		color = { 0.0f,0.85f,0.95f,1.0f };
		for (int i = 0; i < 3; i++) {
			std::unique_ptr<BaseBlock> block;
			block = std::make_unique<BaseBlock>();
			block->Initialize();
			block->SetCollisionManager(cMana_);
			block->GetModel()->SetParent(&transform);
			if (i == 0) block->GetModel()->transform.translate = { 0.0f,2.0f,0.0f };
			if (i == 1) block->GetModel()->transform.translate = { 0.0f,4.0f,0.0f };
			if (i == 2) block->GetModel()->transform.translate = { 0.0f,6.0f,0.0f };
			block->GetModel()->SetColor(color);
			blocks_.push_back(std::move(block));
		}
		break;
	default:
		break;
	}

	// 中心のブロックを生成
	{
		auto& block = blocks_.emplace_back(std::make_unique<BaseBlock>());
		block->Initialize();
		block->SetCollisionManager(cMana_);
		block->GetModel()->SetParent(&transform);
		block->GetModel()->SetColor(color);
	}
}

void Mino::OnCollisionEnter([[maybe_unused]] const ColliderInfo& other) {}

void Mino::OnCollisionStay([[maybe_unused]] const ColliderInfo& other) {}

void Mino::OnCollisionExit([[maybe_unused]] const ColliderInfo& other) {}

Trans& Mino::GetTransform() {
	return transform;
}

const Trans& Mino::GetTransform() const {
	return transform;
}

void Mino::SetCollisionMana(CollisionManager* cMana) {
	cMana_ = cMana;
}
