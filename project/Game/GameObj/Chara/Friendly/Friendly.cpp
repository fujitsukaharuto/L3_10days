#include "Friendly.h"

#include "GameObj/CharaManagers/EnemyManager/EnemyManager.h"

Friendly::Friendly(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {
	moveDir_ = { 1.0f,0.0f,0.0f };
	state_ = State::Search;
}

void Friendly::Update() {
	BaseChara::Update();

}

void Friendly::Search() {
	const auto& enemies = ene_->GetEnemies();

	for (auto& enemy : enemies) {

		// 死んでいたらターゲットにしない
		if (!enemy->GetIsAlive())continue;

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
			state_ = State::Approach;
			return;
		}

	}
}

void Friendly::SetEne(EnemyManager* ene) {
	ene_ = ene;
}