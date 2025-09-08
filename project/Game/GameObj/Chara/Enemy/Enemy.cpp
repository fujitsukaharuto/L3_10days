#include "Enemy.h"

#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"

Enemy::Enemy(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {
	OriginGameObject::GetModel()->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	moveDir_ = { -1.0f,0.0f,0.0f };
	state_ = State::Search;
}

void Enemy::Update() {
	BaseChara::Update();
}

void Enemy::Search() {
	const auto& friendlies = fri_->GetFriendlies();

	for (auto& friendly : friendlies) {

		if (!friendly->GetIsAlive()) continue;

		// 敵の座標を取得
		const Vector3 pos = friendly->GetModel()->GetWorldPos();

		// 敵との距離ベクトル
		Vector3 diff = pos - collider.pos;

		// 距離の二乗
		float distSq = diff.Dot(diff);

		// 半径の二乗
		float radiusSq = collider.radius * collider.radius;

		if (distSq <= radiusSq) {
			SetTarget(friendly.get());
			state_ = State::Approach;
			return;
		}

	}
}
