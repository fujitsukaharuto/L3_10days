#include "GenderBlock.h"

GenderBlock::GenderBlock() {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");
}

void GenderBlock::SetGenderType(const GenderType& type) {
	currentGender_ = type;
}

void GenderBlock::Draw(Material* mate, [[maybe_unused]] bool is) {
	// ブロックなしなら描画しない
	if (currentGender_ == GenderType::NONE) return;

	// ブロックの状態に応じて色を変える
	Vector4 color{};
	switch (currentGender_) {
	case GenderType::DEFAULT:
		color = { 1.0f,1.0f,1.0f,0.5f };
		break;
	case GenderType::MEN:
		color = { 0.0f,0.0f,1.0f,0.5f };
		break;
	case GenderType::WOMEN:
		color = { 1.0f,0.0f,0.0f,0.5f };
		break;
	}
	OriginGameObject::GetModel()->SetColor(color);

	// 描画 (加算合成)
	OriginGameObject::Draw(mate, true);

}