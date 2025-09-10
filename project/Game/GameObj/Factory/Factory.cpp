#include "Factory.h"

Factory::Factory(const Vector3& pos, bool isfriend ) {
	OriginGameObject::Initialize();
	if (isfriend) {
		OriginGameObject::CreateModel("myFactry.obj");
	} else {
		OriginGameObject::CreateModel("enemyFactry.obj");
	}
	OriginGameObject::GetModel()->transform.translate = pos;
	OriginGameObject::GetModel()->transform.translate.y = 8.0f;
	OriginGameObject::GetModel()->transform.scale = { 1.0f,1.0f,20.0f };

	state_ = FactoryState::IDLE;

}

void Factory::Update() {

}

void Factory::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);
}
