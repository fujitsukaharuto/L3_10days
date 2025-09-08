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
}

void BaseChara::Update() {
	// コライダーの座標を更新
	collider.pos = OriginGameObject::GetModel()->GetWorldPos();

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

		break;

	}

	// 移動
	OriginGameObject::GetModel()->transform.translate += velocity * FPSKeeper::DeltaTime();
}

void BaseChara::Draw(Material* mate, bool is) {
	OriginGameObject::Draw(mate, is);

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

		break;
	case WOMEN:

		break;
	}
}

void BaseChara::CheckIsDeadTarget() {

}
