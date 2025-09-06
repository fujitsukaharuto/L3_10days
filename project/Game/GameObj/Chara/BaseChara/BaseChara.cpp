#include "BaseChara.h"

BaseChara::BaseChara(const std::string& modelName, const Vector3& popPos) {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel(modelName);

	OriginGameObject::GetModel()->transform.translate = popPos;

	hp_ = kMaxHp_;

}

BaseChara::~BaseChara() {}

void BaseChara::Update() {

}

void BaseChara::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);

}

void BaseChara::Attack() {

}
