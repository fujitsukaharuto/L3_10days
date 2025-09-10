#include "Factory.h"

Factory::Factory(const Vector3& pos) {
	OriginGameObject::Initialize();
	OriginGameObject::CreateModel("cube.obj");
	OriginGameObject::GetModel()->transform.translate = pos;
	OriginGameObject::GetModel()->transform.scale = { 1.0f,1.0f,20.0f };

	state_ = FactoryState::IDLE;

}

void Factory::Update() {

}

void Factory::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);
}
