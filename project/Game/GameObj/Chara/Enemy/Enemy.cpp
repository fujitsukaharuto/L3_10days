#include "Enemy.h"

#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"

Enemy::Enemy(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {
	OriginGameObject::GetModel()->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	state_ = State::Search;
}

void Enemy::Update() {

	BaseChara::Update();

	// 状態判定
	if (target_) {
		// ターゲットがいた場合アプローチ状態にする
		state_ = State::Approach;

	} else {
		state_ = State::Search;
	}
	// 移動量計算
	Vector3 velocity{};

	// 状態に応じた更新処理
	switch (state_) {
		case State::Search:
			Search();
			// 移動
			velocity = kMoveDir_.Normalize() * speed_;
			OriginGameObject::GetModel()->transform.translate += velocity * FPSKeeper::DeltaTime();
			break;
		case State::Approach:
			// ターゲットした敵に近づいていく

			break;
		case State::Fight:

			break;
	}
}

void Enemy::Search() {
	const auto& friendlies = fri_->GetFriendlies();

	for (auto& friendly : friendlies) {
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
			return;
		}

	}
}

void Enemy::SetFri(FriendlyManager* fri) {
	fri_ = fri;
}
