#include "Mino.h"

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

void Mino::InitBlock(GenderType gender) {
}

Trans& Mino::GetTransform() {
	return transform;
}

const Trans& Mino::GetTransform() const {
	return transform;
}
