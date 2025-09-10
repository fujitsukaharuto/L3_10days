#include "Factory.h"

Factory::Factory(const Vector3& pos, bool isfriend) {
	OriginGameObject::Initialize();
	if (isfriend) {
		OriginGameObject::CreateModel("myFactry.obj");
	} else {
		OriginGameObject::CreateModel("enemyFactry.obj");
	}
	OriginGameObject::GetModel()->transform.translate = pos;
	OriginGameObject::GetModel()->transform.translate.y = 8.0f;

	state_ = FactoryState::IDLE;
	animation = SimpleAnimation<Vector3>(popMinScale_, popMaxScale_, EasingType::EaseInOutBounce, true, LoopType::PingPong);
}

void Factory::Pop() {
	isPop_ = true;
	popTimer_ = popTime_;
}

void Factory::Update() {
	if (isPop_) {
		popTimer_ -= FPSKeeper::GetInstance()->DeltaTimeFrame();
		float t = (popTime_ - popTimer_) / popTime_ * 2.0f;
		OriginGameObject::GetModel()->transform.scale = animation.GetValue(t);
		if (popTimer_ <= 0.0f) {
			isPop_ = false;
		}
	} else {
		OriginGameObject::GetModel()->transform.scale = popMinScale_;

	}
}

void Factory::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);
}

