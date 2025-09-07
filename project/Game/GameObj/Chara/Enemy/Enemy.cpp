#include "Enemy.h"

Enemy::Enemy(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {
	OriginGameObject::GetModel()->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

}

void Enemy::Update() {
	// 移動量計算
	const Vector3 velocity = kMoveDir_.Normalize() * speed_;

	// 移動
	OriginGameObject::GetModel()->transform.translate += velocity * FPSKeeper::DeltaTime();
}
