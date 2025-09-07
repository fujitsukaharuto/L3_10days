#include "Friendly.h"

#include "GameObj/CharaManagers/EnemyManager/EnemyManager.h"

Friendly::Friendly(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {
}

void Friendly::Update() {

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

void Friendly::Search() {
	const auto& enemies = ene_->GetEnemies();

	for (auto& enemy : enemies) {
		// 敵の座標を取得
		const Vector3 pos = enemy->GetModel()->GetWorldPos();

		// 敵との距離ベクトル
		Vector3 diff = pos - collider.pos;

		// 距離の二乗
		float distSq = diff.Dot(diff);

		// 半径の二乗
		float radiusSq = collider.radius * collider.radius;

		if (distSq <= radiusSq) {
			SetTarget(enemy.get());
			return;
		}

	}
}

void Friendly::SetEne(EnemyManager* ene) {
	ene_ = ene;
}