#include "BaseChara.h"

BaseChara::BaseChara(const CharaStatus& status, const Vector3& popPos) {
	// ステータスを受け取る
	status_ = status;

	// ステートを初期化
	state_ = State::Search;

	// 最大hpを取得
	status_.maxHp = status_.hp;

	OriginGameObject::Initialize();
	OriginGameObject::CreateAnimModel(status_.name);
	OriginGameObject::GetAnimModel()->LoadAnimationFile(status.name);
	OriginGameObject::GetAnimModel()->transform.translate = popPos;
	OriginGameObject::GetAnimModel()->transform.scale = { 0.6f,0.6f,0.6f };

	if (status.name == "womanWalk.gltf") {
		OriginGameObject::GetAnimModel()->AddAnimation("womanLose.gltf");
		OriginGameObject::GetAnimModel()->transform.translate.y += 1.0f;
	} else if (status.name == "womanWalk2.gltf") {
		OriginGameObject::GetAnimModel()->AddAnimation("womanLose2.gltf");
		OriginGameObject::GetAnimModel()->AddAnimation("womanRecovery2.gltf");
		OriginGameObject::GetAnimModel()->AddAnimation("womanWin.gltf");
	} else if (status.name == "manWalk.gltf") {
		OriginGameObject::GetAnimModel()->AddAnimation("manLose.gltf");
		OriginGameObject::GetAnimModel()->AddAnimation("manAtack.gltf");
		OriginGameObject::GetAnimModel()->AddAnimation("manWin.gltf");
		OriginGameObject::GetAnimModel()->transform.translate.y -= 0.3f;
	} else if (status.name == "manWalk2.gltf") {
		// OriginGameObject::GetAnimModel()->AddAnimation("manLose2.gltf");
		OriginGameObject::GetAnimModel()->AddAnimation("manAtack2.gltf");
		// 	OriginGameObject::GetAnimModel()->AddAnimation("manWin2.gltf");
		OriginGameObject::GetAnimModel()->transform.translate.y -= 0.3f;
	} else if (status.name == "halfWalk.gltf") {
		OriginGameObject::GetAnimModel()->AddAnimation("halfLose.gltf");
		OriginGameObject::GetAnimModel()->AddAnimation("halfAtack.gltf");
		OriginGameObject::GetAnimModel()->AddAnimation("halfWin.gltf");
	}


	switch (status_.gender) {
		case MAN:
			collider.radius = 20.0f;
			break;

		case WOMAN:
			collider.radius = 20.0f;
			break;
	}

	isAlive_ = true;

	attack = &AudioPlayer::GetInstance()->SoundLoadWave("atack.wav");
	recovery = &AudioPlayer::GetInstance()->SoundLoadWave("recovery.wav");
	die = &AudioPlayer::GetInstance()->SoundLoadWave("die.wav");

}

BaseChara::~BaseChara() {
	AudioPlayer::GetInstance()->SoundPlayWave(*die);
}

void BaseChara::Update() {

	if (!isAlive_) return;

	// コライダーの座標を更新
	collider.pos = OriginGameObject::GetAnimModel()->GetWorldPos();

	// 移動量計算
	Vector3 velocity{};

	// 状態に応じた更新処理
	switch (state_) {
		case State::Search:
		{
			Search();
			// 移動
			velocity = moveDir_.Normalize() * speed_;

			// 回転
			const Vector3 dir = Vector3::Normalize(velocity);
			const float yaw = std::atan2(dir.x, dir.z);
			OriginGameObject::GetAnimModel()->transform.rotate.y = yaw;
		}
		break;
		case State::Approach:
		{
			// 状態判定
			if (!target_) {
				state_ = State::Search;
				if (status_.name == "womanWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.001");
				} else if (status_.name == "womanWalk2.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction");
				} else if (status_.name == "manWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.006");
				} else if (status_.name == "manWalk2.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.001");
				} else if (status_.name == "halfWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.006");
				}
				return;
			}
			// ターゲットした敵に近づいていく

			// ターゲットの座標
			const Vector3 targetPos = target_->GetAnimModel()->transform.translate;
			// 自身の座標
			const Vector3 pos = OriginGameObject::GetAnimModel()->transform.translate;
			// 方向ベクトルを求める
			const Vector3 dir = targetPos - pos;
			// 距離を計算
			const float len = Vector3::Length(dir);

			// 一定距離に入ったら戦闘開始
			if (len < fightRange_) {
				state_ = State::Fight;
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
				return;
			}

			// 正規化
			const Vector3 nDir = Vector3::Normalize(dir);
			// 移動量計算
			velocity = nDir * approachSpeed_;

			// 回転
			const float yaw = std::atan2(dir.x, dir.z);
			OriginGameObject::GetAnimModel()->transform.rotate.y = yaw;
		}
		break;
		case State::Fight:
		{
			// 状態判定
			if (!target_) {
				state_ = State::Search;
				if (status_.name == "womanWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.001");
				} else if (status_.name == "womanWalk2.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction");
				} else if (status_.name == "manWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.006");
				} else if (status_.name == "manWalk2.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.001");
				} else if (status_.name == "halfWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.006");
				}
				return;
			}

			// タイマー処理
			if (actionCoolTimer_ < kActionCoolTime_) {
				actionCoolTimer_ += FPSKeeper::GetInstance()->DeltaTimeFrame();
			} else {
				actionCoolTimer_ = 0.0f;
				Action();
			}

			// ターゲットの座標
			const Vector3 targetPos = target_->GetAnimModel()->transform.translate;
			// 自身の座標
			const Vector3 pos = OriginGameObject::GetAnimModel()->transform.translate;
			// 方向ベクトルを求める
			const Vector3 dir = targetPos - pos;
			// 距離を計算
			const float len = Vector3::Length(dir);

			// 一定距離から離れたら接近フェーズに
			if (len >= fightRange_) {
				actionCoolTimer_ = 0.0f;
				state_ = State::Approach;
				if (status_.name == "womanWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.001");
				} else if (status_.name == "womanWalk2.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction");
				} else if (status_.name == "manWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.006");
				} else if (status_.name == "manWalk2.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.001");
				} else if (status_.name == "halfWalk.gltf") {
					OriginGameObject::GetAnimModel()->ChangeAnimation("amaAction.006");
				}
				return;
			}

			float yaw = std::atan2(dir.x, dir.z);
			OriginGameObject::GetAnimModel()->transform.rotate.y = yaw;
		}
		break;
	}

	// 移動
	OriginGameObject::GetAnimModel()->transform.translate += velocity * FPSKeeper::DeltaTime();
	OriginGameObject::GetAnimModel()->AnimationUpdate();
}

void BaseChara::CSDispatch() {
	OriginGameObject::GetAnimModel()->CSDispatch();
}

void BaseChara::Draw(Material* mate, bool is) {
	is;
	if (animModel_) {
		animModel_->Draw(mate);
	}

}

void BaseChara::GetDamage(int32_t damage) {
	status_.hp -= damage;
	if (status_.hp < 0) {
		status_.hp = 0;
		isAlive_ = false;
	}
}

void BaseChara::GetHeal(int32_t heal) {
	status_.hp += heal;
	if (status_.hp > status_.maxHp) {
		status_.hp = status_.maxHp;
	}
}

void BaseChara::CheckIsTargetDead() {
	if (target_) {
		if (!target_->GetIsAlive()) {
			target_ = nullptr;
		}
	}
}

bool BaseChara::GetIsAlive()const {
	return isAlive_;
}

CharaStatus BaseChara::GetStatus() {
	return status_;
}

void BaseChara::SetTarget(BaseChara* target) {
	target_ = target;
}

void BaseChara::SetIsAlive(bool alive) {
	isAlive_ = alive;
}

void BaseChara::SetEne(EnemyManager* ene) {
	ene_ = ene;
}

void BaseChara::SetFri(FriendlyManager* fri) {
	fri_ = fri;
}

void BaseChara::Action() {
	switch (status_.gender) {
		case MAN:
			if (target_) {
				target_->GetDamage(status_.power);
			}
			AudioPlayer::GetInstance()->SoundPlayWave(*attack);
			break;
		case WOMAN:
			if (target_) {
				target_->GetHeal(status_.power);
				AudioPlayer::GetInstance()->SoundPlayWave(*recovery);
			}
			break;
	}

}