#include "Enemy.h"

#include "GameObj/CharaManagers/FriendlyManager/FriendlyManager.h"
#include "GameObj/CharaManagers/EnemyManager/EnemyManager.h"

Enemy::Enemy(const CharaStatus& status, const Vector3& popPos)
	:BaseChara(status, popPos) {
	moveDir_ = { -1.0f,0.0f,0.0f };
	state_ = State::Search;
	break_ = &AudioPlayer::GetInstance()->SoundLoadWave("break.wav");
}

void Enemy::Update() {
	BaseChara::Update();

	// 勝利判定確認


	if (isWinTime_) {
		winTimer_ += FPSKeeper::GetInstance()->DeltaTimeFrame();
		if (winTimer_ >= winTime_) {
			if (!ene_->GetIsWin()) {
				AudioPlayer::GetInstance()->SoundPlayWave(*break_);
				ene_->Win();
			}
		}
	} else {
		if (OriginGameObject::GetAnimModel()->GetWorldPos().x < winXPos_) {
			isWinTime_ = true;
			winTimer_ = 0.0f;

			if (status_.name == "womanWalk.gltf") {
				OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.002");
			} else if (status_.name == "womanWalk2.gltf") {
				OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.003");
			} else if (status_.name == "manWalk.gltf") {
				OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.003");
			} else if (status_.name == "manWalk2.gltf") {
				OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.003");
			} else if (status_.name == "halfWalk.gltf") {
				OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.004_3");
			}
		}
	}
}

void Enemy::Search() {
	switch (status_.gender) {

		case Gender::MAN:
		{
			const auto& friendlies = fri_->GetFriendlies();
			for (auto& friendly : friendlies) {

				// 死んでいたらターゲットにしない
				if (!friendly->GetIsAlive())continue;

				// 敵の座標を取得
				const Vector3 pos = friendly->GetAnimModel()->GetWorldPos();

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
			break;
		}

		case Gender::WOMAN:
		{
			const auto& enemies = ene_->GetEnemies();
			for (auto& enemy : enemies) {

				// 死んでいたらターゲットにしない
				if (!enemy->GetIsAlive())continue;
				// HPが最大の場合もターゲットにしない
				if (enemy->GetStatus().hp == enemy->GetStatus().maxHp) continue;

				// 目標の座標を取得
				const Vector3 pos = enemy->GetAnimModel()->GetWorldPos();

				// 同じ場合はターゲットにしない
				if (pos == collider.pos)return;

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
		break;
	}

}
