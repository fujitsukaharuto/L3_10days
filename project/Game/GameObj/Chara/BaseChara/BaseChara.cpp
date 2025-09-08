#include "BaseChara.h"

BaseChara::BaseChara(const CharaStatus& status, const Vector3& popPos) {
	// ステータスを受け取る
	status_ = status;

	// ステートを初期化
	state_ = State::Search;

	// 最大hpを取得
	maxHp_ = status_.hp;

	OriginGameObject::Initialize();
	OriginGameObject::CreateModel(status_.name);
	OriginGameObject::GetModel()->transform.translate = popPos;

	collider.radius = 10.0f;

	isAlive_ = true;
}

void BaseChara::Update() {
	
	if (!isAlive_) return;

	// コライダーの座標を更新
	collider.pos = OriginGameObject::GetModel()->GetWorldPos();

	// 状態判定
	if (!target_) {
		state_ = State::Search;
	}

	// 移動量計算
	Vector3 velocity{};

	// 状態に応じた更新処理
	switch (state_) {
	case State::Search:
		Search();
		// 移動
		velocity = moveDir_.Normalize() * speed_;
		break;
	case State::Approach:
	{
		// ターゲットした敵に近づいていく

		// ターゲットの座標
		const Vector3 targetPos = target_->GetModel()->transform.translate;
		// 自身の座標
		const Vector3 pos = OriginGameObject::GetModel()->transform.translate;
		// 方向ベクトルを求める
		const Vector3 dir = targetPos - pos;
		// 距離を計算
		const float len = Vector3::Length(dir);

		// 一定距離に入ったら戦闘開始
		if (len < fightRange_) {
			state_ = State::Fight;
			return;
		}

		// 正規化
		const Vector3 nDir = Vector3::Normalize(dir);
		// 移動量計算
		velocity = nDir * speed_;
	}
	break;
	case State::Fight:
		// タイマー処理
		if (actionCoolTimer_ < kActionCoolTime_) {
			actionCoolTimer_ += FPSKeeper::GetInstance()->DeltaTimeFrame();
		} else {
			actionCoolTimer_ = 0.0f;
			Action();
		}
		break;
	}

	// 移動
	OriginGameObject::GetModel()->transform.translate += velocity * FPSKeeper::DeltaTime();
}

void BaseChara::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);

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
	if (status_.hp > maxHp_) {
		status_.hp = maxHp_;
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

void BaseChara::Action() {
	switch (status_.gender) {
	case MEN:
		if (target_) {
			target_->GetDamage(status_.power);
		}
		break;
	case WOMEN:

		break;
	}
}