#include "Friendly.h"

#include "GameObj/CharaManagers/EnemyManager/EnemyManager.h"
#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"

Friendly::Friendly(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {
	moveDir_ = { 1.0f,0.0f,0.0f };
	state_ = State::Search;
}

void Friendly::Update() {
	BaseChara::Update();
	// 勝利判定確認
	if (OriginGameObject::GetAnimModel()->GetWorldPos().x > winXPos_) {
		if (ene_->GetCurrentWave() != 2) {
			ene_->AddWave();
		} else {
			fri_->Win();
		}
	}
}

void Friendly::Search() {

	switch (status_.gender) {

	case Gender::MAN:
	{
		const auto& enemies = ene_->GetEnemies();
		for (auto& enemy : enemies) {

			// 死んでいたらターゲットにしない
			if (!enemy->GetIsAlive())continue;

			// 敵の座標を取得
			const Vector3 pos = enemy->GetAnimModel()->GetWorldPos();

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
		break;
	}

	case Gender::WOMAN:
	{
		const auto& friendlies = fri_->GetFriendlies();
		for (auto& friendly : friendlies) {

			// 死んでいたらターゲットにしない
			if (!friendly->GetIsAlive())continue;
			// HPが最大の場合もターゲットにしない
			if (friendly->GetStatus().hp == friendly->GetStatus().maxHp) continue;

			// 目標の座標を取得
			const Vector3 pos = friendly->GetAnimModel()->GetWorldPos();

			// 同じ場合はターゲットにしない
			if (pos == collider.pos)return;

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
	break;
	}
}