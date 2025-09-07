#include "Friendly.h"

Friendly::Friendly(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {

}

void Friendly::Update() {
	// 移動量計算
	const Vector3 velocity = kMoveDir_.Normalize() * speed_;

	// 移動
	OriginGameObject::GetModel()->transform.translate += velocity * FPSKeeper::DeltaTime();

}